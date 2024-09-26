#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>

dev_t dev = MKDEV(505,0);
static int init_func(void){
    register_chrdev_region(dev, 1, "My Dynamically allocated device");
    printk(KERN_INFO "Major = %d, minor = %d \n", MAJOR(dev), MINOR(dev));
    printk(KERN_INFO "Kernel Module inserted successfully...\n");
    return 0;
}

static void exit_func(void){
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Kernel Module removed successfully\n");
}

module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nurbol Bizhigit");
MODULE_DESCRIPTION("Simple linux driver (Dynamically allocating the Major and Minor number)");