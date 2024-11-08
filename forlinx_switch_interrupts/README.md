## Please make sure that your dts contains the following lines:
`
/ {
    gpio-leds {
        compatible = "gpio-leds";
        my_gpio_led {
            compatible = "my_gpio_led";   // Matches your kernel module
            label = "my_gpio";
            gpios = <&gpio1 0 GPIO_ACTIVE_LOW>;  // GPIO1_IO00
        };
    };
};
`
