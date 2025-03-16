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

After uploading the code, try shaking the 9DoF up and
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
  
  imu.dmpBegin(DMP_FEATURE_PEDOMETER);
  imu.dmpSetPedometerSteps(stepCount);
  imu.dmpSetPedometerTime(stepTime);
}

void loop() 
{
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
}

