#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define mem_size 1024
#define MIN(a, b) ((a) < (b) ? (a) : (b))

dev_t dev = 0;
static struct class *dev_class;
static struct cdev dev_cdev;
uint8_t *kernel_buffer;


// Function prototypes
static int __init ops_init(void);
static void ops_exit(void);
static int fs_open(struct inode *inode, struct file *file);
static int fs_release(struct inode *inode, struct file *file);
static ssize_t fs_read(struct file *f, char __user *buf, size_t len, loff_t * off);
static ssize_t fs_write(struct file *f, const char *buf, size_t len, loff_t * off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = fs_read,
    .write = fs_write,
    .open = fs_open,
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
    if(*off >= mem_size)
        return 0;
    ssize_t br = MIN(len, mem_size - *off);
    if(copy_to_user(buf, kernel_buffer + *off, br))
        pr_err("Data read: Err!\n");
    pr_info("Driver read: Done\n");
    *off += br;
    return br;
}
static ssize_t fs_write(struct file *f, const char *buf, size_t len, loff_t * off){
    if(copy_from_user(kernel_buffer, buf, len))
        pr_err("Data write: Err!\n");
    pr_info("Driver write function called\n");
    return len;
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

    if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0){
        pr_info("Couldn't allocate memory in kernel\n");
        goto r_device;
    }

    strcpy(kernel_buffer, "Hello_world\n");
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
MODULE_DESCRIPTION("Simple linux device driver with File Operations");





