/*
 * Driver for the Analog Devices digital potentiometers (I2C bus)
 *
 * Copyright (C) 2010-2011 Michael Hennerich, Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of.h>
#include "ad525x_dpot.h"

/* I2C bus functions */
static int write_d8(void *client, u8 val)
{
	return i2c_smbus_write_byte(client, val);
}

static int write_r8d8(void *client, u8 reg, u8 val)
{
	return i2c_smbus_write_byte_data(client, reg, val);
}

static int write_r8d16(void *client, u8 reg, u16 val)
{
	return i2c_smbus_write_word_data(client, reg, val);
}

static int read_d8(void *client)
{
	return i2c_smbus_read_byte(client);
}

static int read_r8d8(void *client, u8 reg)
{
	return i2c_smbus_read_byte_data(client, reg);
}

static int read_r8d16(void *client, u8 reg)
{
	return i2c_smbus_read_word_data(client, reg);
}

static const struct ad_dpot_bus_ops bops = {
	.read_d8	= read_d8,
	.read_r8d8	= read_r8d8,
	.read_r8d16	= read_r8d16,
	.write_d8	= write_d8,
	.write_r8d8	= write_r8d8,
	.write_r8d16	= write_r8d16,
};

static int ad_dpot_i2c_probe(struct i2c_client *client,
				      const struct i2c_device_id *id)
{
	struct ad_dpot_bus_data bdata = {
		.client = client,
		.bops = &bops,
	};

	if (!i2c_check_functionality(client->adapter,
				     I2C_FUNC_SMBUS_WORD_DATA)) {
		dev_err(&client->dev, "SMBUS Word Data not Supported\n");
		return -EIO;
	}

	return ad_dpot_probe(&client->dev, &bdata, id->driver_data, id->name);
}

static int ad_dpot_i2c_remove(struct i2c_client *client)
{
	return ad_dpot_remove(&client->dev);
}

static const struct i2c_device_id ad_dpot_id[] = {
};
MODULE_DEVICE_TABLE(i2c, ad_dpot_id);

#ifdef CONFIG_OF
static const struct of_device_id ad_dpot_of_match[] = {
	{ .compatible = "adi,ad5258", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5259", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5251", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5252", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5253", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5254", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5255", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5241", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5242", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5243", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5245", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5246", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5247", .data = (void *) AD5160_ID, },	
	{ .compatible = "adi,ad5248", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5280", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5282", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,adn2860", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5273", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5161", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5171", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5170", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5172", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5173", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5272", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5274", .data = (void *) AD5160_ID, },
	{/* Sentinel */}
};
MODULE_DEVICE_TABLE(of, ad_dpot_of_match);
#endif

static struct i2c_driver ad_dpot_i2c_driver = {
	.driver = {
		.name	= "ad_dpot",
		.of_match_table	= of_match_ptr(ad_dpot_of_match),
	},
	.probe		= ad_dpot_i2c_probe,
	.remove		= ad_dpot_i2c_remove,
	.id_table	= ad_dpot_id,

};

module_i2c_driver(ad_dpot_i2c_driver);

MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
MODULE_DESCRIPTION("digital potentiometer I2C bus driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("i2c:ad_dpot");
