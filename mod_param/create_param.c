#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>


int value, callback_value, arr_val[4];
char *str;    

module_param(value, int, S_IRUSR| S_IWUSR);
module_param(str, charp, S_IRUSR| S_IWUSR);
module_param_array(arr_val, int, NULL, S_IRUSR| S_IWUSR);

static int notify(const char *val, const struct kernel_param *kp){
    int res = param_set_int(val, kp);
    if(res == 0){
        printk(KERN_INFO "Call back function called...\n");
        printk(KERN_INFO "New value of the callback_value is: %d\n", callback_value);
        return 0;
    }
    return -1;
}
const struct kernel_param_ops ops = {
    .set = &notify,
    .get = &param_get_int, // standard getter
};

module_param_cb(callback_value, &ops, &callback_value, S_IRUGO|S_IWUSR);

static int __init init_func(void){
    printk(KERN_INFO "Value:%d\n", value);
    printk(KERN_INFO "String:%s\n", str);
    printk(KERN_INFO "Callback value:%d\n", callback_value);
    
    for(size_t i = 0; i < sizeof(arr_val)/sizeof(int); ++i){
        printk(KERN_INFO "Array value[%zu] = %d\n", i, arr_val[i]);
    }
    return 0;
}

static void __exit exit_func(void){
    printk(KERN_INFO "Kernel exitted successfully\n");
}

module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nurbol Bizhigit");
MODULE_DESCRIPTION("A practice module for module parameter loading");
MODULE_VERSION("v0.0.1");
