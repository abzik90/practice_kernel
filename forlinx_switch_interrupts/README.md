## Please make sure that your dts contains the following lines:

`/ {
    gpio-leds {\n
        compatible = "gpio-leds";\n
        my_gpio_led {\n
            compatible = "my_gpio_led";   // Matches your kernel module \n
            label = "my_gpio";\n
            gpios = <&gpio1 0 GPIO_ACTIVE_LOW>;  // GPIO1_IO00\n
        };
    };
};`

