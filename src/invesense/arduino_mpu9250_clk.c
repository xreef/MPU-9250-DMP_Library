/******************************************************************************
arduino_mpu9250_clk.c - MPU-9250 Digital Motion Processor Arduino Library 
******************************************************************************/
#include "../invesense/arduino_mpu9250_clk.h"

#include <Arduino.h>

int arduino_get_clock_ms(unsigned long *count)
{
	*count = millis();
	return 0;
}

int arduino_delay_ms(unsigned long num_ms)
{
	delay(num_ms);
	return 0;
}
