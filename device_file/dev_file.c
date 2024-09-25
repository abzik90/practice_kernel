#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/device.h>

dev_t dev = 0;
static struct class *dev_class;

static int __init dev_practice_init(void){
    if(alloc_chrdev_region(&dev, 0, 1, "practice_dev") < 0){
        pr_err("Major number couldn't be assigned for the device");
        return -1;
    }

    pr_info("Major = %d, minor = %d\n", MAJOR(dev), MINOR(dev));

    dev_class = class_create("practice_dev_class");
    if(IS_ERR(dev_class)){
        pr_err("Struct class can't be created\n");
        goto r_class;
    }
    if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "practice_device"))){
        pr_err("Device can't be created\n");
        goto r_device;
    }
    pr_info("Kernel module inserted successfully\n");
    return 0;
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit dev_practice_exit(void){
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    unregister_chrdev_region(dev, 1);
    pr_info("Kernel module unloaded successfully\n");
}

module_init(dev_practice_init);
module_exit(dev_practice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nurbol Bizhigit");
MODULE_DESCRIPTION("Simple linux driver (Automatically Creating a Device file)");