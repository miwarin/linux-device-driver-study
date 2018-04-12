#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "MyDevice"

static const unsigned int MINOR_BASE = 0;
static const unsigned int MINOR_NUM = 2;

static unsigned int mydevice_major;

static struct cdev mydevice_cdev;

static struct class* mydevice_class = NULL;

#define NUM_BUFFER 256
struct _mydevice_file_data {
	unsigned char buffer[NUM_BUFFER];
};

static int mydevice_open(struct inode* inode, struct file* fp)
{
	printk("mydevice_open");

	struct _mydevice_file_data* p = kmalloc(sizeof(struct _mydevice_file_data), GFP_KERNEL);
	if(p == NULL){
		printk(KERN_ERR "kmalloc\n");
		return -ENOMEM;
	}

	strlcat(p->buffer, "dummy", 5);

	fp->private_data = p;

	return 0;
}

static int mydevice_close(struct inode* inode, struct file* fp)
{
	printk("mydevice_close");

	if(fp->private_data) {
		kfree(fp->private_data);
		fp->private_data = NULL;
	}

	return 0;
}

static int mydevice_read(struct file* fp, char __user* buf, size_t count, loff_t* f_pos)
{
	printk("mydevice_read");

	if(count > NUM_BUFFER) {
		count = NUM_BUFFER;
	}

	struct _mydevice_file_data* p = fp->private_data;
	if(copy_to_user(buf, p->buffer, count) != 0) {
		return -EFAULT;
	}

	return count;
}

static int mydevice_write(struct file* fp, const char __user* buf, size_t count, loff_t* f_pos)
{
	printk("mydevice_write");

	struct _mydevice_file_data* p = fp->private_data;
	if(copy_from_user(p->buffer, buf, count) != 0) {
		return -EFAULT;
	}

	return count;
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

	mydevice_class = class_create(THIS_MODULE, "mydevice");
	if(IS_ERR(mydevice_class)) {
		printk(KERN_ERR "class_create\n");
		cdev_del(&mydevice_cdev);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	for(int minor = MINOR_BASE; minor < MINOR_BASE + MINOR_NUM; minor++) {
		device_create(mydevice_class, NULL, MKDEV(mydevice_major, minor), NULL, "mydevice%d", minor);
	}


	return 0;
}

static void mydevice_exit(void)
{
	printk("mydevice_exit\n");

	dev_t dev = MKDEV(mydevice_major, MINOR_BASE);

	for(int minor = MINOR_BASE; minor < MINOR_BASE + MINOR_NUM; minor++) {
		device_destroy(mydevice_class, MKDEV(mydevice_major, minor));
	}

	class_destroy(mydevice_class);

	cdev_del(&mydevice_cdev);

	unregister_chrdev_region(dev, MINOR_NUM);
}

module_init(mydevice_init);
module_exit(mydevice_exit);


