#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>
#include <asm/io.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "MyDevice"

struct dentry* debug_entry_dir;
static int debug_prm1;
static int debug_read_size = 0;


static int mydevice_open(struct inode* inode, struct file* fp)
{
	printk("mydevice_open");
	debug_read_size = 4;
	return 0;
}

static int mydevice_read(struct file* fp, char __user* buf, size_t count, loff_t* f_pos)
{
	printk("mydevice_read");

	if(debug_read_size > 0) {
		int len;
		len = sprintf(buf, "%d\n", debug_prm1);
		debug_read_size -= 4;
		return len;
	}
	else {
		return 0;
	}

	return 0;

}

static int mydevice_write(struct file* fp, const char __user* buf, size_t count, loff_t* f_pos)
{
	printk("mydevice_write");
	sscanf(buf, "%d", &debug_prm1);
	return count;
}

struct file_operations s_mydevice_fops = {
	.owner = THIS_MODULE,
	.open = mydevice_open,
	.read = mydevice_read,
	.write = mydevice_write,
};

static int mydevice_init(void)
{
	printk("mydevice_init");

	debug_entry_dir = debugfs_create_dir(DRIVER_NAME, NULL);
	if(debug_entry_dir == NULL) {
		printk(KERN_ERR "debugfs_create_dir\n");
		return -ENOMEM;
	}

	debugfs_create_file("prm1", S_IRUGO | S_IWUGO, debug_entry_dir, NULL, &s_mydevice_fops);

	debugfs_create_u32("_prm1", S_IRUGO | S_IWUGO, debug_entry_dir, &debug_prm1);

	debugfs_create_x32("_prm1_hex", S_IRUGO | S_IWUGO, debug_entry_dir, &debug_prm1);

	return 0;
}

static void mydevice_exit(void)
{
	printk("mydevice_exit\n");
	debugfs_remove_recursive(debug_entry_dir);
}

module_init(mydevice_init);
module_exit(mydevice_exit);


