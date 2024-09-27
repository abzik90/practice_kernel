#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/cdev.h>
#include<linux/device.h>

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

int32_t value = 0;
dev_t dev = 0;
static struct class *dev_class;
static struct cdev dev_cdev;

// Function prototypes
static int __init ops_init(void);
static void ops_exit(void);
static int fs_open(struct inode *inode, struct file *file);
static int fs_release(struct inode *inode, struct file *file);
static ssize_t fs_read(struct file *f, char __user *buf, size_t len, loff_t * off);
static ssize_t fs_write(struct file *f, const char *buf, size_t len, loff_t * off);
static ssize_t fs_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = fs_read,
    .write = fs_write,
    .open = fs_open,
    .unlocked_ioctl = fs_ioctl,
    .release = fs_release,
};
static int fs_open(struct inode *inode, struct file *file){
    pr_info("Driver Open function called\n");
    return 0;
}
static int fs_release(struct inode *inode, struct file *file){
    pr_info("Driver Release function called\n");
    return 0;
}
static ssize_t fs_read(struct file *f, char __user *buf, size_t len, loff_t * off){
    pr_info("Driver read function called\n");
    return 0;
}
static ssize_t fs_write(struct file *f, const char *buf, size_t len, loff_t * off){
    pr_info("Driver write function called\n");
    return len;
}
static long fs_ioctl(struct file *f, unsigned int cmd, unsigned long arg){
    switch (cmd){
    case WR_VALUE:
        if(copy_from_user(&value, (int32_t *) arg, sizeof(value)))
            pr_err("Data write: ERR!\n");
        pr_info("Value = %d\n", value);
        break;
    case RD_VALUE:
        if(copy_to_user((int32_t *) arg, &value, sizeof(value)))
            pr_err("Data read: ERR!\n");
        break;
    default:
        pr_info("default");
        break;
    }
    return 0; 
}

// Init function
static int __init ops_init(void){
    if(alloc_chrdev_region(&dev, 0, 1, "dummy_driver") < 0){
        pr_err("Unable to allocate major number\n");
        return -1;
    }
    pr_info("Major num: %d, minor num: %d\n", MAJOR(dev), MINOR(dev));
    cdev_init(&dev_cdev, &fops);
    if(cdev_add(&dev_cdev, dev, 1) < 0){
        pr_err("Unable to connect the device to the system\n");
        goto r_class;
    }
    if(IS_ERR(dev_class = class_create("device_class"))){
        pr_err("Unable to create class struct\n");
        goto r_class;
    }
    if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "dummy_device"))){
        pr_err("Unable to create the device\n");
        goto r_device;
    }

    pr_info("Module init successfull\n");
    return 0;
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

// Exit function
static void ops_exit(void){
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&dev_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Module exited successfully\n");
}

module_init(ops_init);
module_exit(ops_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nurbol Bizhigit");
MODULE_DESCRIPTION("Simple linux device driver(IOCTL)");





