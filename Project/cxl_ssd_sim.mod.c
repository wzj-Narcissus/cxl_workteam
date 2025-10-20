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
	{ 0xbf0e63c7, "misc_register" },
	{ 0xf1de9e85, "vfree" },
	{ 0xc5e973a2, "misc_deregister" },
	{ 0x39240082, "vmalloc_to_page" },
	{ 0xa96d32ba, "__udelay" },
	{ 0x44decd6f, "hugetlb_optimize_vmemmap_key" },
	{ 0xf9e3f552, "vm_insert_page" },
	{ 0x44decd6f, "devmap_managed_key" },
	{ 0xbfee9301, "__folio_put" },
	{ 0x0feb1e94, "usleep_range_state" },
	{ 0x5d3bb1dd, "__put_devmap_managed_folio_refs" },
	{ 0x1b3db703, "param_ops_uint" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xd7a59a65, "vzalloc_noprof" },
	{ 0x70eca2ca, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xbf0e63c7,
	0xf1de9e85,
	0xc5e973a2,
	0x39240082,
	0xa96d32ba,
	0x44decd6f,
	0xf9e3f552,
	0x44decd6f,
	0xbfee9301,
	0x0feb1e94,
	0x5d3bb1dd,
	0x1b3db703,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0xd7a59a65,
	0x70eca2ca,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"misc_register\0"
	"vfree\0"
	"misc_deregister\0"
	"vmalloc_to_page\0"
	"__udelay\0"
	"hugetlb_optimize_vmemmap_key\0"
	"vm_insert_page\0"
	"devmap_managed_key\0"
	"__folio_put\0"
	"usleep_range_state\0"
	"__put_devmap_managed_folio_refs\0"
	"param_ops_uint\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"vzalloc_noprof\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "664B885EC606BFEF75B0493");
