#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <asm/uaccess.h>

#define DRIVER_NAME "MyDevice_NAME"
#define DRIVER_MAJOR 63

static int myDevice_open(struct inode* inode, struct file* file)
{
	printk("myDevice_open");
	return 0;
}

static int myDevice_close(struct inode* inode, struct file* file)
{
	printk("myDevice_close");
	return 0;
}

static ssize_t myDevice_read(struct file* filep, char __user* buf, size_t count, loff_t* f_pos)
{
	printk("myDevice_read");
	buf[0] = 'A';
	return 1;
}

static ssize_t myDevice_write(struct file* filep, const char __user* buf, size_t count, loff_t* f_pos)
{
	printk("myDevice_write");
	return 1;
}

struct file_operations s_myDevice_fops = {
	.open = myDevice_open,
	.release = myDevice_close,
	.read = myDevice_read,
	.write = myDevice_write,
};

static int myDevice_init(void)
{
	printk("myDevice_init\n");
	register_chrdev(DRIVER_MAJOR, DRIVER_NAME, &s_myDevice_fops);
	return 0;
}

static void myDevice_exit(void)
{
	printk("myDevice_exit");
	unregister_chrdev(DRIVER_MAJOR, DRIVER_NAME);
}

module_init(myDevice_init);
module_exit(myDevice_exit);


