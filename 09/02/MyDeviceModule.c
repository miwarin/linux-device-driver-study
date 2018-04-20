#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "MyDevice"


/*
 * ピン番号ではなくBCM番号を指定する
 * https://pinout.xyz/
 * */
#define GPIO_PIN_LED 14
#define GPIO_PIN_BTN 15

static irqreturn_t mydevice_gpio_intr(int irq, void *dev_id)
{
	printk("%s\n", __func__);

	int btn;
	btn = gpio_get_value(GPIO_PIN_BTN);
	printk("button = %d\n", btn);
	return IRQ_HANDLED;
}

static int mydevice_init(void)
{
	printk("%s\n", __func__);

	/* 出力に設定する。初期出力は Low */
	gpio_direction_output(GPIO_PIN_LED, 0);

	/* Highにする */	
	gpio_set_value(GPIO_PIN_LED, 1);

	gpio_direction_input(GPIO_PIN_BTN);

	int irq = gpio_to_irq(GPIO_PIN_BTN);
	printk("gpio_to_irq = %d\n", irq);

	if (request_irq(irq, (void*)mydevice_gpio_intr, 
		IRQF_SHARED |            /* 割り込みを複数のデバイスで共有する */
		IRQF_TRIGGER_RISING |    /* 立ち上がりを検知？ */ 
		IRQF_TRIGGER_FALLING,    /* 立ち下がりを検知？ */ 
		"mydevice_gpio_intr", 
		(void*)mydevice_gpio_intr) < 0) {
		
		printk(KERN_ERR "request_irq\n");
		return -1;
	}

	return 0;
}

static void mydevice_exit(void)
{
	printk("%s\n", __func__);
	int irq = gpio_to_irq(GPIO_PIN_BTN);
	free_irq(irq, (void*)mydevice_gpio_intr);
}

module_init(mydevice_init);
module_exit(mydevice_exit);


