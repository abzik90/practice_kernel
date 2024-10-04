#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x5c658724, "cdev_init" },
	{ 0xa2dc7695, "cdev_add" },
	{ 0x38e073, "class_create" },
	{ 0x23a131f9, "device_create" },
	{ 0xc7a86bcd, "kernel_kobj" },
	{ 0xd6eed201, "kobject_create_and_add" },
	{ 0xdff48634, "sysfs_create_file_ns" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x9901e5e3, "class_destroy" },
	{ 0x22aed8b0, "kobject_put" },
	{ 0xbdd41ab6, "sysfs_remove_file_ns" },
	{ 0x37a0cba, "kfree" },
	{ 0x2f8479f2, "device_destroy" },
	{ 0xd58dd2b1, "cdev_del" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x4c03a563, "random_kmalloc_seed" },
	{ 0x91a35752, "kmalloc_caches" },
	{ 0x34004e19, "kmalloc_trace" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xf079b8f9, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D88EF5383DFB9E35764EA79");
