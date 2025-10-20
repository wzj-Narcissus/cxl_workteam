// cxl_ssd_sim.c - Minimal CXL-SSD-style memory semantic simulator (fixed)
// Build: make
// Load: sudo insmod cxl_ssd_sim.ko mem_mb=64 delay_us=2000
// Test: ./user_test
// Unload: sudo rmmod cxl_ssd_sim

#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/uaccess.h>

#define DEV_NAME "ssdmem0"

static unsigned int mem_mb = 64;       // 模拟设备容量（MB）
module_param(mem_mb, uint, 0644);
MODULE_PARM_DESC(mem_mb, "Simulated memory size in MB");

static unsigned int delay_us = 2000;   // 每次缺页注入的延迟（微秒）
module_param(delay_us, uint, 0644);
MODULE_PARM_DESC(delay_us, "Per-page-fault delay in microseconds");

// 设备上下文
struct ssdmem_ctx {
    void   *buf;           // vmalloc 的大缓冲区
    size_t  size;          // 字节数
};
static struct ssdmem_ctx g_ctx;

// -------- 缺页处理：在这里注入延迟，并建立映射 ----------
static vm_fault_t ssdmem_fault(struct vm_fault *vmf)
{
    struct vm_area_struct *vma = vmf->vma;
    unsigned long addr = (unsigned long)vmf->address;

    /* 计算映射中的总字节偏移： (addr - vm_start) + (pgoff << PAGE_SHIFT) */
    unsigned long vma_off = addr - vma->vm_start;
    unsigned long buf_off = vma_off + ((unsigned long)vmf->pgoff << PAGE_SHIFT);

    void *buf_addr;
    struct page *page;
    int ret;

    if (buf_off >= g_ctx.size)
        return VM_FAULT_SIGBUS;

    buf_addr = g_ctx.buf + (buf_off & PAGE_MASK);
    page = vmalloc_to_page(buf_addr);
    if (!page)
        return VM_FAULT_SIGBUS;

    // 模拟CXL-SSD访问延迟（微秒级）
    if (delay_us) {
        if (delay_us < 2000) {
            udelay(delay_us);
        } else {
            usleep_range(delay_us, delay_us + 100);
        }
    }

    get_page(page); // 提升引用计数，提交给VM
    ret = vmf_insert_page(vma, addr, page);
    if (ret == 0)
        return VM_FAULT_NOPAGE;
    if (ret == -ENOMEM)
        return VM_FAULT_OOM;
    if (ret == -EAGAIN)
        return VM_FAULT_RETRY;

    put_page(page);
    return VM_FAULT_SIGBUS;
}

static const struct vm_operations_struct ssdmem_vm_ops = {
    .fault = ssdmem_fault,
};

// -------- mmap：把缺页处理器挂到VMA上 ----------
static int ssdmem_mmap(struct file *filp, struct vm_area_struct *vma)
{
    // 新内核里 vm_flags 是只读的，这两个flag对最小demo不是必须，先不改
    // vm_flags_set(vma, VM_DONTEXPAND | VM_DONTDUMP); // 如需可换成 helper
    vma->vm_ops = &ssdmem_vm_ops;

    pr_info("ssdmem: mmap VMA [0x%lx, 0x%lx), pgoff=%lu\n",
            vma->vm_start, vma->vm_end, vma->vm_pgoff);
    return 0;
}

static const struct file_operations ssdmem_fops = {
    .owner = THIS_MODULE,
    .mmap  = ssdmem_mmap,
};

static struct miscdevice ssdmem_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = DEV_NAME,
    .fops  = &ssdmem_fops,
};

// -------- 模块加载/卸载 ----------
static int __init ssdmem_init(void)
{
    int ret;
    size_t bytes = (size_t)mem_mb * 1024ull * 1024ull;

    if (bytes == 0 || (bytes & (PAGE_SIZE - 1))) {
        pr_err("ssdmem: mem_mb must yield a non-zero, page-aligned size\n");
        return -EINVAL;
    }

    g_ctx.buf = vzalloc(bytes);
    if (!g_ctx.buf) {
        pr_err("ssdmem: vzalloc %zu bytes failed\n", bytes);
        return -ENOMEM;
    }
    g_ctx.size = bytes;

    ret = misc_register(&ssdmem_miscdev);
    if (ret) {
        vfree(g_ctx.buf);
        pr_err("ssdmem: misc_register failed %d\n", ret);
        return ret;
    }

    pr_info("ssdmem: loaded. /dev/%s size=%u MB delay_us=%u\n",
            DEV_NAME, mem_mb, delay_us);
    return 0;
}

static void __exit ssdmem_exit(void)
{
    misc_deregister(&ssdmem_miscdev);
    if (g_ctx.buf) {
        vfree(g_ctx.buf);
        g_ctx.buf = NULL;
    }
    pr_info("ssdmem: unloaded\n");
}

module_init(ssdmem_init);
module_exit(ssdmem_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("you");
MODULE_DESCRIPTION("Minimal CXL-SSD-like memory semantic simulator (fixed)");


