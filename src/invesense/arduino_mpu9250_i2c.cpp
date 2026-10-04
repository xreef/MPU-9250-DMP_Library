/******************************************************************************
arduino_mpu9250_i2c.cpp - MPU-9250 Digital Motion Processor Arduino Library
******************************************************************************/
#include "../invesense/arduino_mpu9250_i2c.h"

#include <Arduino.h>
#include <Wire.h>

// Both functions return 0 on success and -1 on an I2C error, so the
// InvenSense driver (and begin()) can detect a missing or wrong device.

int arduino_i2c_write(unsigned char slave_addr, unsigned char reg_addr,
                       unsigned char length, unsigned char * data)
{
	Wire.beginTransmission(slave_addr);
	Wire.write(reg_addr);
	for (unsigned char i = 0; i < length; i++)
	{
		Wire.write(data[i]);
	}

	return (Wire.endTransmission(true) == 0) ? 0 : -1;
}

int arduino_i2c_read(unsigned char slave_addr, unsigned char reg_addr,
                       unsigned char length, unsigned char * data)
{
	Wire.beginTransmission(slave_addr);
	Wire.write(reg_addr);
	// The result of a repeated-start endTransmission(false) is not checked:
	// some cores (e.g. older ESP32 ones) return a non-zero "continue" code
	// even on success. A NACK still shows up as a short requestFrom().
	Wire.endTransmission(false);
	if (Wire.requestFrom(slave_addr, length) != length)
	{
		return -1;
	}
	for (unsigned char i = 0; i < length; i++)
	{
		data[i] = Wire.read();
	}

	return 0;
}
