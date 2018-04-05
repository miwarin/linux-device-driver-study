#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <asm/current.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "MyDevice"

static const unsigned int MINOR_BASE = 0;
static const unsigned int MINOR_NUM = 2;

static unsigned int mydevice_major;

static struct cdev mydevice_cdev;



static int mydevice_open(struct inode* inode, struct file* file)
{
	printk("mydevice_open");
	return 0;
}

static int mydevice_close(struct inode* inode, struct file* file)
{
	printk("mydevice_close");
	return 0;
}

static int mydevice_read(struct file* filp, char __user* buf, size_t count, loff_t* f_pos)
{
	printk("mydevice_read");
	buf[0] = 'A';
	return 1;
}

static int mydevice_write(struct file* filp, const char __user* buf, size_t count, loff_t* f_pos)
{
	printk("mydevice_write");
	return 1;
}

struct file_operations s_mydevice_fops = {
	.open = mydevice_open,
	.release = mydevice_close,
	.read = mydevice_read,
	.write = mydevice_write,
};

static int mydevice_init(void)
{
	printk("mydevice_init");

	int alloc_ret = 0;
	int cdev_err = 0;
	dev_t dev;

	alloc_ret = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
	if(alloc_ret != 0) {
		printk(KERN_ERR "alloc_chrdev_region = %d\n", alloc_ret);
		return -1;
	}

	mydevice_major = MAJOR(dev);
	dev = MKDEV(mydevice_major, MINOR_BASE);

	cdev_init(&mydevice_cdev, &s_mydevice_fops);
	mydevice_cdev.owner = THIS_MODULE;

	cdev_err = cdev_add(&mydevice_cdev, dev, MINOR_NUM);
	if(cdev_err != 0) {
		printk(KERN_ERR "cdev_add = %d\n", cdev_err);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	return 0;
}

static void mydevice_exit(void)
{
	printk("mydevice_exit\n");

	dev_t dev = MKDEV(mydevice_major, MINOR_BASE);

	cdev_del(&mydevice_cdev);

	unregister_chrdev_region(dev, MINOR_NUM);
}

module_init(mydevice_init);
module_exit(mydevice_exit);


