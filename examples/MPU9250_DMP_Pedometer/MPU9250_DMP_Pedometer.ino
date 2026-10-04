/************************************************************
MPU9250_DMP_Pedometer
 Pedometer example for MPU-9250 DMP Arduino Library

 Renzo Mischianti @ mischianti.org
 https://github.com/xreef/MPU-9250-DMP_Library

Jim Lindblom @ SparkFun Electronics
original creation date: November 23, 2016
https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

The MPU-9250's digital motion processor (DMP) can estimate
steps taken -- effecting a pedometer.

After uploading the code, try shaking the board up and
down at a "stepping speed."

*************************************************************/
#include <MPU9250-DMP.h>

#if defined(SAMD)
#define SerialPort SerialUSB
#else
#define SerialPort Serial
#endif

MPU9250_DMP imu;

unsigned long stepCount = 0;
unsigned long stepTime = 0;
unsigned long lastStepCount = 0;

void setup()
{
  SerialPort.begin(115200);
  delay(2000); // Wait a bit for the serial monitor to open
  SerialPort.println("Starting...");

  // Call imu.begin() to verify communication and initialize
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

  // Enable the DMP pedometer and start counting from zero
  // steps and zero walking time.
  imu.dmpBegin(DMP_FEATURE_PEDOMETER);
  imu.dmpSetPedometerSteps(stepCount);
  imu.dmpSetPedometerTime(stepTime);

  // NOTE: The pedometer algorithm requires several consecutive, rhythmic
  // steps (typically 5-7) before it starts counting. Single isolated
  // movements are ignored to prevent false positives.
}

void loop()
{
  // The step count and the walking time (ms) are read directly from
  // the DMP; no FIFO read is needed.
  stepCount = imu.dmpGetPedometerSteps();
  stepTime = imu.dmpGetPedometerTime();

  if (stepCount != lastStepCount)
  {
    lastStepCount = stepCount;
    SerialPort.print("Walked " + String(stepCount) +
                     " steps");
    SerialPort.println(" (" +
              String((float)stepTime / 1000.0) + " s)");
  }

  delay(100); // Prevents saturating the I2C bus with continuous reads
}
