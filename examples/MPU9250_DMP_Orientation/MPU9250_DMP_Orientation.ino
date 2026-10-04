/************************************************************
MPU9250_DMP_Orientation
 Orientation example for MPU-9250 DMP Arduino Library

 Renzo Mischianti @ mischianti.org
 https://github.com/xreef/MPU-9250-DMP_Library

Jim Lindblom @ SparkFun Electronics
original creation date: November 23, 2016
https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

Uses the MPU-9250's digital motion processing engine to
determine the orientation of the board (portrait/landscape,
like a phone screen). A message is printed every time the
orientation changes.

*************************************************************/
#include <MPU9250-DMP.h>

#if defined(SAMD)
#define SerialPort SerialUSB
#else
#define SerialPort Serial
#endif

MPU9250_DMP imu;

// Mounting matrix: how the sensor axes are aligned with the board.
// The identity matrix means the sensor and the board axes match.
const signed char orientationMatrix[9] = {
  1, 0, 0,
  0, 1, 0,
  0, 0, 1
};
unsigned char lastOrient = 0;

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

  // Enable the DMP orientation feature and apply the mounting matrix.
  imu.dmpBegin(DMP_FEATURE_ANDROID_ORIENT);
  imu.dmpSetOrientation(orientationMatrix);
}

void loop()
{
  // The orientation is updated each time the DMP FIFO is read.
  if ( imu.fifoAvailable() )
  {
    imu.dmpUpdateFifo();
    unsigned char orient = imu.dmpGetOrientation();
    if (orient != lastOrient)
    {
      switch (orient)
      {
      case ORIENT_PORTRAIT:
        SerialPort.println("Portrait");
        break;
      case ORIENT_LANDSCAPE:
        SerialPort.println("Landscape");
        break;
      case ORIENT_REVERSE_PORTRAIT:
        SerialPort.println("Portrait (Reverse)");
        break;
      case ORIENT_REVERSE_LANDSCAPE:
        SerialPort.println("Landscape (Reverse)");
        break;
      }
      lastOrient = orient;
    }
  }
  delay(10); // Prevents I2C spam on fast microcontrollers like ESP32
}
