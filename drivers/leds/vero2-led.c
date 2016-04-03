/*
 * vero2-led.c -- Vero 2 Front LED driver
 *
 * Copyright (C) 2016 Sam Nazarko <email@samnazarko.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 */

#include <linux/leds.h>
#include <linux/module.h>
#include <linux/amlogic/aml_gpio_consumer.h>
#include <linux/amlogic/gpio-amlogic.h>
#include <plat/io.h>
#include <mach/io.h>
#include <mach/register.h>

MODULE_AUTHOR("Sam Nazarko");
MODULE_DESCRIPTION("Vero 2 front LED driver");
MODULE_LICENSE("GPL");

#define LED_ON 1
#define LED_OFF 0

static void toggle_led(int status)
{
	if (status == LED_OFF)
            aml_clr_reg32_mask(P_AO_GPIO_O_EN_N,1 << 31);
	else
	    aml_set_reg32_mask(P_AO_GPIO_O_EN_N,1 << 31);
}

static void vero2_led_set(struct led_classdev *led_cdev,
		enum led_brightness value)
{
	if (value == LED_OFF)
		toggle_led(LED_OFF);
	else
	   	toggle_led(LED_ON);
}

static struct led_classdev vero2_led = {
	.name		= "vero2::front",
	.brightness	= LED_OFF,
	.max_brightness = 1,
	.brightness_set = vero2_led_set,
	.flags		= LED_CORE_SUSPENDRESUME,
};

static int __init vero2_led_init(void)
{
	toggle_led(LED_OFF); // Off by default
	return led_classdev_register(NULL, &vero2_led);
}

static void __exit vero2_led_exit(void)
{
	led_classdev_unregister(&vero2_led);
	toggle_led(LED_ON); // On when removed
}

module_init(vero2_led_init);
module_exit(vero2_led_exit);
