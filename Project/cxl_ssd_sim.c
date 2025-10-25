// cxl_ssd_sim.c — safer + portable kmap (local/atomic) fallback
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/highmem.h>     // ✅ kmap_local_page / kmap_atomic
#include <linux/version.h>     // ✅ LINUX_VERSION_CODE

#define DEV_NAME "ssdmem0"

static unsigned int mem_mb = 8;       // 默认 8MB，小一点更安全
module_param(mem_mb, uint, 0644);
static unsigned int delay_us = 100;   // 默认 100us
module_param(delay_us, uint, 0644);

struct ssdmem_ctx {
    void   *buf;   // 后端镜像缓冲（vmalloc）
    size_t  size;
};
static struct ssdmem_ctx g_ctx;

/* 兼容层：新内核用 kmap_local_page，旧内核退回 kmap_atomic */
#if defined(kmap_local_page) && defined(kunmap_local) && (LINUX_VERSION_CODE >= KERNEL_VERSION(5,15,0))
# define kmap_compat(pg)       kmap_local_page(pg)
# define kunmap_compat(addr)   kunmap_local(addr)
#else
# define kmap_compat(pg)       kmap_atomic(pg)
# define kunmap_compat(addr)   kunmap_atomic(addr)
#endif

static vm_fault_t ssdmem_fault(struct vm_fault *vmf)
{
    struct vm_area_struct *vma = vmf->vma;
    unsigned long addr    = (unsigned long)vmf->address;
    unsigned long vma_off = addr - vma->vm_start;
    unsigned long buf_off = vma_off + ((unsigned long)vmf->pgoff << PAGE_SHIFT);
    struct page *page;
    void *kaddr;

    if (buf_off >= g_ctx.size)
        return VM_FAULT_SIGBUS;

    if (delay_us) {
        if (delay_us < 2000) udelay(delay_us);
        else                 usleep_range(delay_us, delay_us + 100);
    }

    page = alloc_page(GFP_HIGHUSER | __GFP_ZERO);
    if (!page)
        return VM_FAULT_OOM;

    kaddr = kmap_compat(page);
    memcpy(kaddr, g_ctx.buf + (buf_off & PAGE_MASK), PAGE_SIZE);
    kunmap_compat(kaddr);

    if (vmf_insert_page(vma, addr, page) == 0)
        return VM_FAULT_NOPAGE;

    put_page(page);
    return VM_FAULT_SIGBUS;
}

static const struct vm_operations_struct ssdmem_vm_ops = {
    .fault = ssdmem_fault,
};

static int ssdmem_mmap(struct file *filp, struct vm_area_struct *vma)
{
    vma->vm_ops = &ssdmem_vm_ops;
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

static int __init ssdmem_init(void)
{
    size_t bytes = (size_t)mem_mb * 1024ull * 1024ull;
    int ret;

    if (!bytes || (bytes & (PAGE_SIZE - 1)))
        return -EINVAL;

    g_ctx.buf = vzalloc(bytes);
    if (!g_ctx.buf)
        return -ENOMEM;
    g_ctx.size = bytes;

    ret = misc_register(&ssdmem_miscdev);
    if (ret) {
        vfree(g_ctx.buf);
        return ret;
    }
    pr_info("ssdmem: /dev/%s ready. size=%uMB delay_us=%u\n", DEV_NAME, mem_mb, delay_us);
    return 0;
}

static void __exit ssdmem_exit(void)
{
    misc_deregister(&ssdmem_miscdev);
    if (g_ctx.buf) vfree(g_ctx.buf);
    pr_info("ssdmem: unloaded\n");
}

module_init(ssdmem_init);
module_exit(ssdmem_exit);
MODULE_LICENSE("GPL");

