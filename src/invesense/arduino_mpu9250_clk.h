/******************************************************************************
arduino_mpu9250_clk.h - MPU-9250 Digital Motion Processor Arduino Library 
******************************************************************************/
#ifndef _ARDUINO_MPU9250_CLK_H_
#define _ARDUINO_MPU9250_CLK_H_

int arduino_get_clock_ms(unsigned long *count);
int arduino_delay_ms(unsigned long num_ms);

#endif // _ARDUINO_MPU9250_CLK_H_
