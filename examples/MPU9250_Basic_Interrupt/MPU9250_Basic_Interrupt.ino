/************************************************************
MPU9250_Basic_DataReady
Basic example for the MPU-9250 DMP Arduino Library that uses the sensor's
"data ready" signal to indicate when to re-read sensor data.
Note: This is not a traditional hardware interrupt; it simply signals that new
data is available for reading.

Renzo Mischianti @ www.mischianti.org
Original creation date: November 23, 2016
https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

Supported Platforms:
- ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
*************************************************************/
#include <MPU9250-DMP.h>

#define INTERRUPT_PIN 1  // Pin used to monitor the sensor's data-ready signal

MPU9250_DMP imu;

void setup()
{
  // Configure the interrupt pin with an internal pull-up resistor.
  // The sensor will pull this pin LOW when new data is available.
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  Serial.begin(115200);

  // Initialize the MPU-9250; if initialization fails, halt execution.
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  // Configure the sensor to use the gyroscope, accelerometer, and magnetometer,
  // and set their sample rates to 4Hz (slow rate to easily observe the data-ready signal).
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  imu.setSampleRate(4);          // Set accelerometer/gyroscope sample rate to 4Hz
  imu.setCompassSampleRate(4);   // Set magnetometer sample rate to 4Hz

  // Enable the sensor's data-ready signal.
  // This activates an output that indicates when a new set of data is available.
  imu.enableInterrupt();

  // Set the interrupt signal polarity to active-low.
  // With an internal pull-up resistor enabled, the signal will be HIGH when idle
  // and LOW when new data is ready.
  imu.setIntLevel(INT_ACTIVE_LOW);

  // Configure the interrupt mode as latched.
  // In latching mode, once the data-ready signal goes LOW, it remains LOW until the data is read.
  // This is used solely to flag that a fresh set of sensor data is available.
  imu.setIntLatched(INT_LATCHED);
}

void loop()
{
  // Check the state of the data-ready indicator.
  // A LOW state signifies that new data is ready to be read.
  if (digitalRead(INTERRUPT_PIN) == LOW)
  {
    // Update the sensor readings from the MPU-9250.
    // This fetches the latest accelerometer, gyroscope, and magnetometer data.
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    printIMUData();
  }
}

void printIMUData(void)
{
  // Convert raw sensor readings (16-bit signed values) into human-readable units.
  // Accelerometer in g, Gyroscope in degrees per second, Magnetometer in microteslas.
  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);

  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);

  float magX = imu.calcMag(imu.mx);
  float magY = imu.calcMag(imu.my);
  float magZ = imu.calcMag(imu.mz);

  // Note: If gyroscope values always display 0, verify that the gyroscope sensor
  // is properly enabled and configured in the setSensors() function.

  // Print the sensor data in a neat tabular format:
  Serial.println("-----------------------------------------------------");
  Serial.print("Time (ms): "); Serial.println(imu.time);
  Serial.println("-----------------------------------------------------");
  Serial.print("Accel (g):     ");
  Serial.print(String(accelX, 2)); Serial.print("  |   ");
  Serial.print(String(accelY, 2)); Serial.print(" |  ");
  Serial.println(String(accelZ, 2));
  Serial.print("Gyro (dps):   ");
  Serial.print(String(gyroX, 2)); Serial.print("  |   ");
  Serial.print(String(gyroY, 2)); Serial.print(" |   ");
  Serial.println(String(gyroZ, 2));
  Serial.print("Mag (uT):   ");
  Serial.print(String(magX, 2)); Serial.print("  | ");
  Serial.print(String(magY, 2)); Serial.print(" | ");
  Serial.println(String(magZ, 2));
  Serial.println("-----------------------------------------------------\n");
}
