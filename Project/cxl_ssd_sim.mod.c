#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x690fb1ed, "class_destroy" },
	{ 0x52b15b3b, "__unregister_chrdev" },
	{ 0xa254dac2, "__free_pages" },
	{ 0xcb8b6ec6, "kfree" },
	{ 0xa59da3c0, "down_write" },
	{ 0xa59da3c0, "up_write" },
	{ 0x5e865cb8, "pgprot_writecombine" },
	{ 0xd710adbf, "__kmalloc_noprof" },
	{ 0x6077a174, "alloc_pages_noprof" },
	{ 0x78a12d13, "__register_chrdev" },
	{ 0x3cd2d66b, "class_create" },
	{ 0x2035aec5, "device_create" },
	{ 0x2d4f4516, "param_ops_uint" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x44decd6f, "hugetlb_optimize_vmemmap_key" },
	{ 0xa96d32ba, "__udelay" },
	{ 0x5d1ebb98, "device_destroy" },
	{ 0xd268ca91, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x690fb1ed,
	0x52b15b3b,
	0xa254dac2,
	0xcb8b6ec6,
	0xa59da3c0,
	0xa59da3c0,
	0x5e865cb8,
	0xd710adbf,
	0x6077a174,
	0x78a12d13,
	0x3cd2d66b,
	0x2035aec5,
	0x2d4f4516,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0x44decd6f,
	0xa96d32ba,
	0x5d1ebb98,
	0xd268ca91,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"class_destroy\0"
	"__unregister_chrdev\0"
	"__free_pages\0"
	"kfree\0"
	"down_write\0"
	"up_write\0"
	"pgprot_writecombine\0"
	"__kmalloc_noprof\0"
	"alloc_pages_noprof\0"
	"__register_chrdev\0"
	"class_create\0"
	"device_create\0"
	"param_ops_uint\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"hugetlb_optimize_vmemmap_key\0"
	"__udelay\0"
	"device_destroy\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "64F2B4D750178905C3A1A79");
