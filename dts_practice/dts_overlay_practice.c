#include<linux/module.h>
#include<linux/init.h>
#include<linux/mod_devicetable.h>
#include<linux/property.h>
#include<linux/platform_device.h>
#include<linux/of_device.h>

MODULE_LICENSE("GPL");
// Original from Johannes 4 GNU/Linux
MODULE_AUTHOR("Nurbol Bizhigit");
MODULE_DESCRIPTION("Simple LKM to parse device trees for specific device and its properties");

static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);

static struct of_device_id driver_ids[] = {
    {
    .compatible = "my_custom_device",
    },{}
};
MODULE_DEVICE_TABLE(of, driver_ids);

static struct platform_driver dummy_driver = {
    .probe = dt_probe,
    .remove = dt_remove,
    .driver = {
        .name = "dummy_device_driver",
        .of_match_table = driver_ids,
    },
};

static int dt_probe(struct platform_device *pdev) {
    struct device *dev = &pdev->dev;
    const char *label;
    int value;

    pr_info("I'm inside the probe function\n");

    if(!device_property_present(dev, "label")){
        pr_err("dt_probe: ERROR! Dev property 'label' is not found\n");
        return -1;
    }

    if(!device_property_present(dev, "value")){
        pr_err("dt_probe: ERROR! Dev property 'value' is not found\n");
        return -1;
    }

    if(device_property_read_string(dev, "label", &label)){
        pr_err("dt_probe: ERROR! Couldn't read 'label'\n");
        return -1;
    }

    if(device_property_read_u32(dev, "value", &value)){
        pr_err("dt_probe: ERROR! Couldn't read 'value'\n");
        return -1;
    }

    pr_info("dt_probe: \n\t%s\n\t%d\n", label, value);
    return 0;
}

static int dt_remove(struct platform_device *pdev){
    pr_info("dt_probe: Inside the remove function\n");
    return 0;
}

static int __init dt_init(void){
    pr_info("Loading the driver...\n");
    if(platform_driver_register(&dummy_driver)){
        pr_err("dt_probe: ERROR! Couldn't load the driver\n");
        return -1;
    }
    return 0;
}

static void __exit dt_exit(void){
    pr_info("Unloading the driver\n");
    platform_driver_unregister(&dummy_driver);
}

module_init(dt_init);
module_exit(dt_exit);
