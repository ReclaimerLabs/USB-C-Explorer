/*
 * tcpm_driver.c
 *
 * Created: 11/11/2017 18:42:26
 *  Author: jason
 */ 

#include "tcpm_driver.h"

extern const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT];

/* I2C wrapper functions - get I2C port / slave addr from config struct. */
int tcpc_write(int port, int reg, int val)
{
	uint8_t data[2];
	data[0] = (0xFF) & reg;
	data[1] = (0xFF) & val;
	
	struct i2c_master_packet packet;
	packet.address = tcpc_config[port].i2c_slave_addr;
	packet.data_length = 2;
	packet.data = data;
	packet.ten_bit_address = false;
	packet.high_speed = false;
	packet.hs_master_code = 0x00;
	
	return i2c_master_write_packet_wait(&i2c_master_instance, &packet);
}

int tcpc_write16(int port, int reg, int val)
{
	uint8_t data[3];
	data[0] = (0xFF) & reg;
	data[1] = (0xFF) & val;
	data[2] = (0xFF) & (val >> 8);
	
	struct i2c_master_packet packet;
	packet.address = tcpc_config[port].i2c_slave_addr;
	packet.data_length = 3;
	packet.data = data;
	packet.ten_bit_address = false;
	packet.high_speed = false;
	packet.hs_master_code = 0x00;
	
	return i2c_master_write_packet_wait(&i2c_master_instance, &packet);
}

int tcpc_read(int port, int reg, int *val)
{
	uint8_t data[1];
	data[0] = (0xFF) & reg;
	
	struct i2c_master_packet packet;
	packet.address = tcpc_config[port].i2c_slave_addr;
	packet.data_length = 1;
	packet.data = data;
	packet.ten_bit_address = false;
	packet.high_speed = false;
	packet.hs_master_code = 0x00;
	
	enum status_code ret_code = i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &packet);
	if (STATUS_OK != ret_code)
	{
		return ret_code;
	}

	packet.address = tcpc_config[port].i2c_slave_addr;
	packet.data_length = 1;
	packet.data = data;
	packet.ten_bit_address = false;
	packet.high_speed = false;
	packet.hs_master_code = 0x00;

	ret_code = i2c_master_read_packet_wait(&i2c_master_instance, &packet);
	if (STATUS_OK != ret_code)
	{
		return ret_code;
	}
	
	*val = data[0];
	
	return STATUS_OK;
}

int tcpc_read16(int port, int reg, int *val)
{
	uint8_t data[2];
	data[0] = (0xFF) & reg;
	
	struct i2c_master_packet packet;
	packet.address = tcpc_config[port].i2c_slave_addr;
	packet.data_length = 1;
	packet.data = data;
	packet.ten_bit_address = false;
	packet.high_speed = false;
	packet.hs_master_code = 0x00;
	
	enum status_code ret_code = i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &packet);
	if (STATUS_OK != ret_code)
	{
		return ret_code;
	}

	packet.address = tcpc_config[port].i2c_slave_addr;
	packet.data_length = 2;
	packet.data = data;
	packet.ten_bit_address = false;
	packet.high_speed = false;
	packet.hs_master_code = 0x00;

	ret_code = i2c_master_read_packet_wait(&i2c_master_instance, &packet);
	if (STATUS_OK != ret_code)
	{
		return ret_code;
	}
	
	*val  = data[0];
	*val |= (data[1] << 8);
	
	return STATUS_OK;
}

int tcpc_xfer(int port,
	const uint8_t *out, int out_size,
	uint8_t *in, int in_size,
	int flags)
{
	struct i2c_master_packet packet;
	packet.address = tcpc_config[port].i2c_slave_addr;
	packet.data_length = out_size;
	packet.data = out;
	packet.ten_bit_address = false;
	packet.high_speed = false;
	packet.hs_master_code = 0x00;

	enum status_code ret_code;
	if (out_size != 0)
	{
		if (flags & I2C_XFER_STOP)
		{
			ret_code = i2c_master_write_packet_wait(&i2c_master_instance, &packet);
		}
		else
		{
			ret_code = i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &packet);
		}
		if (STATUS_OK != ret_code)
		{
			return ret_code;
		}
	}

	packet.address = tcpc_config[port].i2c_slave_addr;
	packet.data_length = in_size;
	packet.data = in;
	packet.ten_bit_address = false;
	packet.high_speed = false;
	packet.hs_master_code = 0x00;

	if (in_size != 0)
	{
		// We always send a stop, then another start on the next call. 
		// This avoids trying to read after sending a NACK, which indicates the end of the read. 
		// In the future, this could be optimized by using lower level i2c calls. 
		ret_code = i2c_master_read_packet_wait(&i2c_master_instance, &packet);
		if (STATUS_OK != ret_code)
		{
			return ret_code;
		}
	}

	return STATUS_OK;
}
