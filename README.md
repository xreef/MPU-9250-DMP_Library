MPU-9250 DMP Library

Renzo Mischianti detail at [mischianti.org](mischianti.org)


========================================

This library born from a fork of SparkFun MPU-9250 Digital Motion Processor (DMP) Arduino Library
I do this because the original library is not maintained and not support esp32 and other microcontrollers.

========================================

Advanced Arduino library for the Invensense MPU-9250 inertial measurement unit (IMU), which enables the sensor's digital motion processing (DMP) features. Along with configuring and reading from the accelerometer, gyroscope, and magnetometer, this library also supports the chip's DMP features like:

* Quaternion calculation
* Pedometer
* Gyroscope calibration
* Tap detection
* Orientation dtection

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/src** - Source files for the library (.cpp, .h).
	* **/src/util** - Source and headers for the MPU-9250 driver and dmp configuration. These are available and adapted from [Invensene's downloads page](https://www.invensense.com/developers/software-downloads/#sla_content_45).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Changelog
---------------
2024-08-19: v1.0.0 Fist commit of production library

Certainly! Below is an enhanced README section with small, explained code snippets in Markdown syntax for integrating and using the MPU-9250 sensor with an Arduino:

Setting Up the MPU-9250
---------------

To get started with the MPU-9250, include the necessary library and initialize the sensor. This snippet demonstrates the basic setup process:

```cpp
#include <MPU9250-DMP.h>

MPU9250_DMP imu;

void setup() {
  Serial.begin(115200);

  // Initialize the MPU-9250 and check if it's connected properly
  if (imu.begin() != INV_SUCCESS) {
    while (1) {
      Serial.println("Unable to communicate with MPU-9250");
      delay(5000);
    }
  }

  // Enable all sensors (Gyroscope, Accelerometer, Compass)
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
}
```

### Configuring Sensor Settings

You can configure the full-scale ranges for the gyroscope and accelerometer, as well as set up the digital low-pass filter (LPF) and sample rate:

```cpp
// Set gyroscope full scale range to ±2000 degrees per second (dps)
imu.setGyroFSR(2000);

// Set accelerometer full scale range to ±2g
imu.setAccelFSR(2);

// Set digital low-pass filter to 5Hz for smooth data output
imu.setLPF(5);

// Set the sample rate for accelerometer and gyroscope to 10Hz
imu.setSampleRate(10);

// Set the magnetometer (compass) sample rate to 10Hz
imu.setCompassSampleRate(10);
```

### Reading Sensor Data Without FIFO

To read data from the accelerometer, gyroscope, and magnetometer directly (without using the FIFO buffer):

```cpp
void loop() {
  // Check if new data is available from the sensor
  if (imu.dataReady()) {
    // Update the sensor data (Accel, Gyro, Compass)
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);

    // Access and print the updated data
    Serial.println("Accel X: " + String(imu.calcAccel(imu.ax)) + " g");
    Serial.println("Gyro X: " + String(imu.calcGyro(imu.gx)) + " dps");
    Serial.println("Mag X: " + String(imu.calcMag(imu.mx)) + " uT");
  }
}
```

### Using the FIFO for Stable Orientation Data

For applications requiring stable orientation data, you can use the Digital Motion Processor (DMP) with the FIFO buffer to calculate quaternions and Euler angles:

```cpp
// Initialize the DMP to output quaternion data at 10Hz
imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);

void loop() {
  // Check if new data is available in the FIFO
  if (imu.fifoAvailable()) {
    // Update the FIFO and calculate quaternion values
    if (imu.dmpUpdateFifo() == INV_SUCCESS) {
      // Compute Euler angles from the quaternion data
      imu.computeEulerAngles();

      // Print Euler angles (Roll, Pitch, Yaw)
      Serial.print("Roll: "); Serial.println(imu.roll);
      Serial.print("Pitch: "); Serial.println(imu.pitch);
      Serial.print("Yaw: "); Serial.println(imu.yaw);
    }
  }
}
```

### Summary

- **Initialization**: Begin by including the library, initializing the MPU-9250, and enabling the necessary sensors.
- **Configuration**: Customize the gyroscope and accelerometer ranges, set the LPF, and adjust the sample rates.
- **Reading Data**: Directly read the sensor data using `update()` or use the FIFO buffer for stable orientation measurements.
- **Orientation Tracking**: Utilize the DMP and FIFO to compute and track orientation using quaternions and Euler angles.

These examples demonstrate how to use the MPU-9250 in a variety of scenarios, here a basic data acquisition and orientation tracking.