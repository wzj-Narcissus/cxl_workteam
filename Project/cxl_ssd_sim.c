#include <linux/device.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

#define DEVICE_NAME "ssdmem"
#define CLASS_NAME  "cxlssd"

static int major;
static struct class *ssdmem_class;
static struct page **pages;
static unsigned long num_pages;
static unsigned int delay_us = 0;

module_param(delay_us, uint, 0444);
MODULE_PARM_DESC(delay_us, "Artificial delay per page fault (microseconds)");

static unsigned int mem_mb = 8;
module_param(mem_mb, uint, 0444);
MODULE_PARM_DESC(mem_mb, "Total simulated memory size in MB");

// ---------- page fault handler ----------
static vm_fault_t ssdmem_fault(struct vm_fault *vmf)
{
    unsigned long pgoff = vmf->pgoff;
    struct page *page;

    if (pgoff >= num_pages)
        return VM_FAULT_SIGBUS;

    page = pages[pgoff];
    get_page(page);
    vmf->page = page;

    if (delay_us)
        udelay(delay_us);

    return 0;
}

static const struct vm_operations_struct ssdmem_vm_ops = {
    .fault = ssdmem_fault,
};

// ---------- mmap handler ----------
static int ssdmem_mmap(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long size = vma->vm_end - vma->vm_start;

    if (size > num_pages * PAGE_SIZE)
        return -EINVAL;

    vma->vm_ops = &ssdmem_vm_ops;
    vm_flags_set(vma, VM_IO | VM_DONTEXPAND | VM_DONTDUMP);
    vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

    pr_info("ssdmem: mmap size=%lu bytes\n", size);
    return 0;
}

// ---------- open/release ----------
static int ssdmem_open(struct inode *inode, struct file *file)
{
    pr_info("ssdmem: opened\n");
    return 0;
}

static int ssdmem_release(struct inode *inode, struct file *file)
{
    pr_info("ssdmem: released\n");
    return 0;
}

static const struct file_operations ssdmem_fops = {
    .owner = THIS_MODULE,
    .open = ssdmem_open,
    .release = ssdmem_release,
    .mmap = ssdmem_mmap,
};

// ---------- module init ----------
static int __init ssdmem_init(void)
{
    unsigned long i;

    num_pages = (mem_mb * 1024UL * 1024UL) / PAGE_SIZE;
    pages = kmalloc_array(num_pages, sizeof(struct page *), GFP_KERNEL);
    if (!pages)
        return -ENOMEM;

    for (i = 0; i < num_pages; i++) {
        pages[i] = alloc_page(GFP_KERNEL | __GFP_ZERO);
        if (!pages[i])
            return -ENOMEM;
    }

    major = register_chrdev(0, DEVICE_NAME, &ssdmem_fops);
    if (major < 0)
        return major;

    ssdmem_class = class_create(CLASS_NAME);
    if (IS_ERR(ssdmem_class))
        return PTR_ERR(ssdmem_class);

    device_create(ssdmem_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME "0");
    pr_info("ssdmem: /dev/%s0 ready, %u MB, delay=%u us\n",
            DEVICE_NAME, mem_mb, delay_us);
    return 0;
}

// ---------- module exit ----------
static void __exit ssdmem_exit(void)
{
    unsigned long i;

    device_destroy(ssdmem_class, MKDEV(major, 0));
    class_destroy(ssdmem_class);
    unregister_chrdev(major, DEVICE_NAME);

    if (pages) {
        for (i = 0; i < num_pages; i++)
            if (pages[i])
                __free_page(pages[i]);
        kfree(pages);
    }

    pr_info("ssdmem: unloaded\n");
}

module_init(ssdmem_init);
module_exit(ssdmem_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YourTeam");
MODULE_DESCRIPTION("Safe CXL-SSD simulator memory module");

