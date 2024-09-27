#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/proc_fs.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/ioctl.h>
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

int32_t value = 0;
dev_t dev = 0;
char proc_array[20] = "some_proc_array\n";
static int len = 1;
static struct class *dev_class;
static struct cdev dev_cdev;
static struct proc_dir_entry *parent;

// Function prototypes
static int __init ops_init(void);
static void ops_exit(void);
// Driver Functions
static int fs_open(struct inode *inode, struct file *file);
static int fs_release(struct inode *inode, struct file *file);
static ssize_t fs_read(struct file *f, char __user *buf, size_t len, loff_t * off);
static ssize_t fs_write(struct file *f, const char *buf, size_t len, loff_t * off);
static ssize_t fs_ioctl(struct file *f, unsigned int cmd, unsigned long arg);
// Procfs Functions
static int pr_open(struct inode *inode, struct file *file);
static int pr_release(struct inode *inode, struct file *file);
static ssize_t pr_read(struct file *f, char __user *buf, size_t len, loff_t * off);
static ssize_t pr_write(struct file *f, const char *buf, size_t len, loff_t * off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = fs_read,
    .write = fs_write,
    .open = fs_open,
    .unlocked_ioctl = fs_ioctl,
    .release = fs_release,
};

static struct proc_ops pops = {
    .proc_read = pr_read,
    .proc_write = pr_write,
    .proc_open = pr_open,
    .proc_release = pr_release,
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
        pr_info("default\n");
        break;
    }
    return 0; 
}
static int pr_open(struct inode *inode, struct file *file){
    pr_info("Proc file opened\n");
    return 0;
}
static int pr_release(struct inode *inode, struct file *file){
    pr_info("Proc file released\n");
    return 0;
}

static ssize_t pr_read(struct file *f, char __user *buffer, size_t length, loff_t *off){
    pr_info("Proc file read...\n");
    if(len)
        len = 0;
    else{
        len = 1;
        return 0;
    }
    if(copy_to_user(buffer, proc_array, 20))
        pr_err("Data send: ERR!\n");
    return length;
}

static ssize_t pr_write(struct file *f, const char *buffer, size_t length, loff_t *off){
    pr_info("Proc file write...\n");
    if(copy_from_user(proc_array, buffer, length))
        pr_err("Data write: ERR!\n");
    return length;
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

    parent = proc_mkdir("dummy", NULL);
    if(parent == NULL){
        pr_info("Error creating proc entry\n");
        goto r_device;
    }
    proc_create("dummy_proc", 0666, parent, &pops);

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
    proc_remove(parent);
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





