#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/of_platform.h>


MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "MyDevice"

static const struct of_device_id mydevice_of_match_table[] = {
	{.compatible = "mycompany,myoriginaldevice",},
	{ }
};
MODULE_DEVICE_TABLE(of, mydevice_of_match_table);

static struct i2c_device_id mydevice_i2c_idtable[] = {
	{"MyI2CDevice", 0},
	{},
};
MODULE_DEVICE_TABLE(i2c, mydevice_i2c_idtable);

static int mydevice_i2c_probe(struct i2c_client* client, const struct i2c_device_id* id)
{
	printk("%s\n", __func__);

	if(id != NULL)
		printk("id.name = %s id.driver_data = %lu\n", id->name, id->driver_data);
	if(client != NULL)
		printk("slave address = 0x%02x\n", client->addr);

	int version = i2c_smbus_read_byte_data(client, 0x0f);
	printk("id = 0x%02x\n", version);
	return 0;
}

static int mydevice_i2c_remove(struct i2c_client* client)
{
	printk("%s\n", __func__);
	return 0;
}

static struct i2c_driver mydevice_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
	.id_table = mydevice_i2c_idtable,
	.probe = mydevice_i2c_probe,
	.remove = mydevice_i2c_remove,
};

module_i2c_driver(mydevice_driver);


