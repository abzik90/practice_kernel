#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#define DEVICE_NAME "gpio_interrupt"
static int gpio;
static int irq_num;

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    int value = gpio_get_value(gpio);
    pr_info("GPIO interrupt triggered: %s edge detected (GPIO %d)\n", 
            value ? "Rising" : "Falling", gpio);

    return IRQ_HANDLED;
}

static int __init gpio_interrupt_init(void)
{
    struct device_node *leds_node, *my_led_node;
    const char *label;

    // Locate the "gpio-leds" node
    leds_node = of_find_node_by_name(NULL, "gpio-leds");
    if (!leds_node) {
        pr_err("Failed to find gpio-leds node\n");
        return -ENODEV;
    }

    // Locate the "my_gpio_led" subnode
    my_led_node = of_get_child_by_name(leds_node, "my_gpio_led");
    if (!my_led_node) {
        pr_err("Failed to find my_gpio_led node\n");
        of_node_put(leds_node);
        return -ENODEV;
    }

    // Parse the "label" property
    if (of_property_read_string(my_led_node, "label", &label)) {
        pr_err("Failed to read label property\n");
        of_node_put(my_led_node);
        of_node_put(leds_node);
        return -EINVAL;
    }
    pr_info("Label: %s\n", label);

    // Parse the "gpios" property to get the GPIO number
    gpio = of_get_named_gpio(my_led_node, "gpios", 0);
    if (!gpio_is_valid(gpio)) {
        pr_err("Invalid GPIO\n");
        of_node_put(my_led_node);
        of_node_put(leds_node);
        return -EINVAL;
    }
    pr_info("GPIO: %d\n", gpio);

    // Free GPIO if it was previously requested
    gpio_free(gpio);

    // Request GPIO
    if (gpio_request(gpio, label)) {
        pr_err("Failed to request GPIO\n");
        of_node_put(my_led_node);
        of_node_put(leds_node);
        return -EBUSY;
    }

    // Set GPIO as input
    gpio_direction_input(gpio);

    // Get IRQ number for the GPIO
    irq_num = gpio_to_irq(gpio);
    if (irq_num < 0) {
        pr_err("Failed to get IRQ number for GPIO\n");
        gpio_free(gpio);
        of_node_put(my_led_node);
        of_node_put(leds_node);
        return irq_num;
    }

    pr_info("Assigned IRQ number: %d\n", irq_num);

    // Request IRQ for rising and falling edges
    if (request_irq(irq_num, gpio_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                    DEVICE_NAME, NULL)) {
        pr_err("Failed to request IRQ\n");
        gpio_free(gpio);
        of_node_put(my_led_node);
        of_node_put(leds_node);
        return -EIO;
    }

    pr_info("GPIO interrupt module loaded, IRQ attached\n");

    // Release device tree node references
    of_node_put(my_led_node);
    of_node_put(leds_node);

    return 0;
}

static void __exit gpio_interrupt_exit(void)
{
    // Free IRQ and GPIO
    free_irq(irq_num, NULL);
    gpio_free(gpio);

    pr_info("GPIO interrupt module unloaded\n");
}

module_init(gpio_interrupt_init);
module_exit(gpio_interrupt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nurbol Bizhigit");
MODULE_DESCRIPTION("Module to handle GPIO interrupt on rising/falling edges");
