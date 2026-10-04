/************************************************************
MPU9250_FIFO_Basic
 Basic FIFO example for MPU-9250 DMP Arduino Library

 Renzo Mischianti @ mischianti.org
 https://github.com/xreef/MPU-9250-DMP_Library

Jim Lindblom @ SparkFun Electronics
original creation date: November 23, 2016
https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

This example sketch demonstrates how to use the MPU-9250's
512 byte first-in, first-out (FIFO) buffer. The FIFO can be
set to store accelerometer and/or gyroscope data (the
magnetometer cannot be stored in the FIFO by this library).

*************************************************************/
#include <MPU9250-DMP.h>

#if defined(SAMD)
#define SerialPort SerialUSB
#else
#define SerialPort Serial
#endif

MPU9250_DMP imu;

void printIMUData(void);

void setup()
{
  SerialPort.begin(115200);
  delay(2000); // Wait a bit for the serial monitor to open
  SerialPort.println("Starting...");

  // Call imu.begin() to verify communication with and
  // initialize the MPU-9250 to its default values.
  // Most functions return an error code - INV_SUCCESS (0)
  // indicates the IMU was present and successfully set up
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      SerialPort.println("Unable to communicate with MPU-9250");
      SerialPort.println("Check connections, and try again.");
      SerialPort.println();
      delay(5000);
    }
  }

  SerialPort.println("MPU-9250 initialized successfully!");

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  imu.setSampleRate(100); // Set sample rate to 100Hz

  // Use configureFifo to set which sensors should be stored
  // in the buffer.
  // Parameter to this function can be: INV_XYZ_GYRO,
  // INV_XYZ_ACCEL, INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  imu.configureFifo(INV_XYZ_GYRO |INV_XYZ_ACCEL);
}

void loop()
{
  // fifoAvailable returns the number of bytes in the FIFO
  // The FIFO is 512 bytes max. We'll read when it reaches
  // half of that.
  if ( imu.fifoAvailable() >= 256)
  {
    // Then read while there is data in the FIFO
    while ( imu.fifoAvailable() > 0)
    {
      // Call updateFifo to update ax, ay, az, gx, gy, and/or gz
      if ( imu.updateFifo() == INV_SUCCESS)
      {
        printIMUData();
      }
    }
  }
  delay(10); // Prevents I2C spam on fast microcontrollers like ESP32
}

void printIMUData(void)
{
  // After calling updateFifo() the ax, ay, az, gx, gy, gz,
  // and time class variables are all updated.
  // Access them by placing the object. in front:

  // Use the calcAccel and calcGyro functions to
  // convert the raw sensor readings (signed 16-bit values)
  // to their respective units.
  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);
  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);

  SerialPort.println("Accel: " + String(accelX) + ", " +
              String(accelY) + ", " + String(accelZ) + " g");
  SerialPort.println("Gyro: " + String(gyroX) + ", " +
              String(gyroY) + ", " + String(gyroZ) + " dps");
  SerialPort.println("Time: " + String(imu.time) + " ms");
  SerialPort.println();
}
