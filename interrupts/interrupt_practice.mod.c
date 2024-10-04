#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

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
	{ 0xb33c3607, "module_layout" },
	{ 0xf7829730, "cdev_del" },
	{ 0xc2c954e7, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x4b6a90d3, "class_destroy" },
	{ 0xcf710034, "sysfs_remove_file_ns" },
	{ 0xf513389e, "kobject_put" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x4e3fefa9, "sysfs_create_file_ns" },
	{ 0x95ce7032, "kobject_create_and_add" },
	{ 0x2cf487ad, "kernel_kobj" },
	{ 0xad316ae6, "device_create" },
	{ 0x69bd2dc2, "__class_create" },
	{ 0xafa5f4ea, "cdev_add" },
	{ 0xd9d1fe18, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "4E2110AFA1A701BDD6EBFCA");
