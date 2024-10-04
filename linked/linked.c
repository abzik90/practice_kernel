#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/sysfs.h>
#include<linux/kobject.h>
#include<linux/interrupt.h>
#include<linux/uaccess.h>
#include<linux/slab.h> 
// #include<asm/io.h>
#include<linux/err.h>


struct linked_list{
    struct list_head list;
    int data;
};

LIST_HEAD(Head_Node);

volatile int value = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev dev_cdev;
static struct kobject *kobj_ref;

// Function prototypes
static int __init ops_init(void);
static void ops_exit(void);

// static irqreturn_t irq_handler(int irq, void *dev_id){
//     pr_info("Shared IRQ: Interrupt occured");
//     return IRQ_HANDLED;
// }

static int fs_open(struct inode *inode, struct file *file);
static int fs_release(struct inode *inode, struct file *file);
static ssize_t fs_read(struct file *f, char __user *buf, size_t len, loff_t * off);
static ssize_t fs_write(struct file *f, const char *buf, size_t len, loff_t * off);

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
struct kobj_attribute my_attr = __ATTR(value, 0660, sysfs_show, sysfs_store);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = fs_read,
    .write = fs_write,
    .open = fs_open,
    .release = fs_release,
};

static void workqueue_function(void){

    struct linked_list *temp_node = NULL;
    // allocate memory of size linked_list
    temp_node = kmalloc(sizeof(struct linked_list), GFP_KERNEL);
    // add data to the ll
    temp_node->data = value;
    // init list structure in the node itself
    INIT_LIST_HEAD(&temp_node->list);
    // add node to the linked list
    list_add_tail(&temp_node->list, &Head_Node);

}
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
    struct linked_list *temp;
    int count = 0;
    list_for_each_entry(temp, &Head_Node, list){
        pr_info("Node = %d, Data = %d\n", count++, temp->data);
    }
    pr_info("Total Nodes = %d\n", count);
    return 0;
}
static ssize_t fs_write(struct file *f, const char *buf, size_t len, loff_t *off) {
    char kbuf[11]; // A temporary kernel buffer
    int ret;

    pr_info("Driver write function called\n");

    // Ensure we don't overflow the buffer
    if (len > sizeof(kbuf) - 1)
        len = sizeof(kbuf) - 1;

    // Copy data from user space to kernel space
    if (copy_from_user(kbuf, buf, len)) {
        pr_err("Failed to copy data from user space\n");
        return -EFAULT;
    }

    // Null-terminate the kernel buffer
    kbuf[len] = '\0';

    // Now we can safely use sscanf on the kernel buffer
    ret = sscanf(kbuf, "%d", &value);
    if (ret != 1) {
        pr_err("Failed to parse input\n");
        return -EINVAL;
    }

    // Call the workqueue function
    workqueue_function();

    return len;
}

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    pr_info("SysFS read\n");
    // asm("int $0x3B");
    return sprintf(buf, "%d", value);
}
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    pr_info("SysFS write\n");
    sscanf(buf, "%d\n", &value);
    return count;
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
    kobj_ref = kobject_create_and_add("dummy_sysfs_2", kernel_kobj);
    if(sysfs_create_file(kobj_ref, &my_attr.attr)){
        pr_err("Unable to create sysfs file!\n");
        goto r_sysfs;
    }
    
    // if(request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "dummy_device", (void *)(irq_handler))){
    //     pr_err("Unable to register IRQ");
    //     goto irq;
    // }
    pr_info("Module init successfull\n");
    return 0;

// irq:
//     free_irq(IRQ_NO, (void *)(irq_handler));
r_sysfs:
    kobject_put(kobj_ref);
    sysfs_remove_file(kernel_kobj, &my_attr.attr);
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

// Exit function
static void ops_exit(void){
    // free_irq(IRQ_NO, (void *)(irq_handler));
    struct linked_list *cursor, *temp;
    list_for_each_entry_safe(cursor, temp, &Head_Node, list){
        list_del(&cursor->list);
        kfree(cursor);
    }
    kobject_put(kobj_ref);
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





