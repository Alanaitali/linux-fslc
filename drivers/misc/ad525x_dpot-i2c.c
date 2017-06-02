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
	{"ad5258", AD5258_ID},
	{"ad5259", AD5259_ID},
	{"ad5251", AD5251_ID},
	{"ad5252", AD5252_ID},
	{"ad5253", AD5253_ID},
	{"ad5254", AD5254_ID},
	{"ad5255", AD5255_ID},
	{"ad5241", AD5241_ID},
	{"ad5242", AD5242_ID},
	{"ad5243", AD5243_ID},
	{"ad5245", AD5245_ID},
	{"ad5246", AD5246_ID},
	{"ad5247", AD5247_ID},
	{"ad5248", AD5248_ID},
        {"ad5263", AD5263_ID},
	{"ad5280", AD5280_ID},
	{"ad5282", AD5282_ID},
	{"adn2860", ADN2860_ID},
	{"ad5273", AD5273_ID},
	{"ad5161", AD5161_ID},
	{"ad5171", AD5171_ID},
	{"ad5170", AD5170_ID},
	{"ad5172", AD5172_ID},
	{"ad5173", AD5173_ID},
	{"ad5272", AD5272_ID},
	{"ad5274", AD5274_ID},
	{}
};
MODULE_DEVICE_TABLE(i2c, ad_dpot_id);

 
#ifdef CONFIG_OF
static const struct of_device_id ad_dpot_of_match[] = {
	{ .compatible = "adi,ad5160", .data = (void *) AD5160_ID, },
	{ .compatible = "adi,ad5161", .data = (void *) AD5161_ID, },
	{ .compatible = "adi,ad5162", .data = (void *) AD5162_ID, },
	{ .compatible = "adi,ad5165", .data = (void *) AD5165_ID, },
	{ .compatible = "adi,ad5200", .data = (void *) AD5200_ID, },
	{ .compatible = "adi,ad5201", .data = (void *) AD5201_ID, },
	{ .compatible = "adi,ad5203", .data = (void *) AD5203_ID, },
	{ .compatible = "adi,ad5204", .data = (void *) AD5204_ID, },
	{ .compatible = "adi,ad5206", .data = (void *) AD5206_ID, },
	{ .compatible = "adi,ad5207", .data = (void *) AD5207_ID, },
	{ .compatible = "adi,ad5231", .data = (void *) AD5231_ID, },	
	{ .compatible = "adi,ad5232", .data = (void *) AD5232_ID, },
	{ .compatible = "adi,ad5233", .data = (void *) AD5233_ID, },	
	{ .compatible = "adi,ad5235", .data = (void *) AD5235_ID, },
	{ .compatible = "adi,ad5260", .data = (void *) AD5260_ID, },
	{ .compatible = "adi,ad5262", .data = (void *) AD5262_ID, },
	{ .compatible = "adi,ad5263", .data = (void *) AD5263_ID, },
	{ .compatible = "adi,ad5290", .data = (void *) AD5290_ID, },	
	{ .compatible = "adi,ad5291", .data = (void *) AD5291_ID, },
	{ .compatible = "adi,ad5292", .data = (void *) AD5292_ID, },	
	{ .compatible = "adi,ad5293", .data = (void *) AD5293_ID, },
	{ .compatible = "adi,ad7376", .data = (void *) AD7376_ID, },
	{ .compatible = "adi,ad8400", .data = (void *) AD8400_ID, },
	{ .compatible = "adi,ad8402", .data = (void *) AD8402_ID, },	
	{ .compatible = "adi,ad8403", .data = (void *) AD8403_ID, },
	{ .compatible = "adi,adn2850", .data = (void *) ADN2850_ID, },
	{ .compatible = "adi,ad5270", .data = (void *) AD5270_ID, },
	{ .compatible = "adi,ad5272", .data = (void *) AD5272_ID, },	
	{/* Sentinel */}
};
MODULE_DEVICE_TABLE(of, ad_dpot_of_match);
#endif

static struct i2c_driver ad_dpot_i2c_driver = {
	.driver = {
		.name	= "ad_dpot",
		.owner	= THIS_MODULE,
	},
	.probe		= ad_dpot_i2c_probe,
	.remove		= ad_dpot_i2c_remove,
	.id_table	= ad_dpot_id,
		.of_match_table	= of_match_ptr(ad_dpot_of_match),
};

module_i2c_driver(ad_dpot_i2c_driver);

MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
MODULE_DESCRIPTION("digital potentiometer I2C bus driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("i2c:ad_dpot");
