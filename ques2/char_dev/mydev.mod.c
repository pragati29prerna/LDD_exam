#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

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
	{ 0xeeab4c1e, "module_layout" },
	{ 0x3c03d27a, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x44d7ad45, "cdev_add" },
	{ 0x462a2181, "cdev_alloc" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x1000e51, "schedule" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0xcf2a6966, "up" },
	{ 0x56470118, "__warn_printk" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x6bd0e573, "down_interruptible" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F328903CF69D7E258D31E68");
