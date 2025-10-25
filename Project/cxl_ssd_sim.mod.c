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
	{ 0xf1de9e85, "vfree" },
	{ 0xe8213e80, "_printk" },
	{ 0xab571464, "misc_deregister" },
	{ 0xa96d32ba, "__udelay" },
	{ 0x6077a174, "alloc_pages_noprof" },
	{ 0x5d05e3c5, "pcpu_hot" },
	{ 0xc9b02247, "const_pcpu_hot" },
	{ 0xbd03ed67, "vmemmap_base" },
	{ 0xbd03ed67, "page_offset_base" },
	{ 0x77047fb4, "vm_insert_page" },
	{ 0x44decd6f, "hugetlb_optimize_vmemmap_key" },
	{ 0x44decd6f, "devmap_managed_key" },
	{ 0xd6cbdf2c, "__put_devmap_managed_folio_refs" },
	{ 0x0feb1e94, "usleep_range_state" },
	{ 0xa86be0a8, "__folio_put" },
	{ 0xd272d446, "__SCT__preempt_schedule" },
	{ 0x2d4f4516, "param_ops_uint" },
	{ 0xd272d446, "__fentry__" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xd7a59a65, "vzalloc_noprof" },
	{ 0x0011321a, "misc_register" },
	{ 0xd268ca91, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xf1de9e85,
	0xe8213e80,
	0xab571464,
	0xa96d32ba,
	0x6077a174,
	0x5d05e3c5,
	0xc9b02247,
	0xbd03ed67,
	0xbd03ed67,
	0x77047fb4,
	0x44decd6f,
	0x44decd6f,
	0xd6cbdf2c,
	0x0feb1e94,
	0xa86be0a8,
	0xd272d446,
	0x2d4f4516,
	0xd272d446,
	0xd272d446,
	0xd7a59a65,
	0x0011321a,
	0xd268ca91,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"vfree\0"
	"_printk\0"
	"misc_deregister\0"
	"__udelay\0"
	"alloc_pages_noprof\0"
	"pcpu_hot\0"
	"const_pcpu_hot\0"
	"vmemmap_base\0"
	"page_offset_base\0"
	"vm_insert_page\0"
	"hugetlb_optimize_vmemmap_key\0"
	"devmap_managed_key\0"
	"__put_devmap_managed_folio_refs\0"
	"usleep_range_state\0"
	"__folio_put\0"
	"__SCT__preempt_schedule\0"
	"param_ops_uint\0"
	"__fentry__\0"
	"__x86_return_thunk\0"
	"vzalloc_noprof\0"
	"misc_register\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B776C7C0F0C7889F52916AA");
