# MPU-9250 DMP Library

**Author:** Renzo Mischianti \
**Website:** [**mischianti.org**](https://mischianti.org/) \
**Arduino Library Registry:** [indexing logs](https://downloads.arduino.cc/libraries/logs/github.com/xreef/MPU-9250-DMP_Library/)

> [!TIP]
> 📖 **Full documentation, wiring diagrams and tutorials are available on [mischianti.org](https://mischianti.org/mpu9250-with-esp32-and-arduino-accelerometer-magnetometer-and-gyroscope-via-i2c-and-spi/).**
> See the [MPU-9250 tutorial series](#-tutorials-on-mischiantiorg) below for step-by-step guides.

> [!NOTE]
> This library is a fork of the **SparkFun MPU-9250 Digital Motion Processor (DMP) Arduino Library**.
> I created it because the original library is no longer maintained and does not support the ESP32 and other microcontrollers.

---

## Table of Contents

- [📖 Tutorials on mischianti.org](#-tutorials-on-mischiantiorg)
- [Overview](#overview)
- [Repository Contents](#repository-contents)
- [Getting Started](#getting-started)
  - [Setting Up the MPU-9250](#setting-up-the-mpu-9250)
  - [Configuring Sensor Settings](#configuring-sensor-settings)
  - [Reading Sensor Data Without FIFO](#reading-sensor-data-without-fifo)
  - [Using the FIFO for Stable Orientation Data](#using-the-fifo-for-stable-orientation-data)
- [Summary](#summary)
- [Changelog](#changelog)

---

## 📖 Tutorials on mischianti.org

In-depth articles on the MPU-9250 and this library, with wiring diagrams for Arduino, ESP32 and other boards, explained examples, and troubleshooting tips.

### MPU-9250 series

| # | Article | Topics |
|---|---------|--------|
| 1 | [TDK InvenSense MPU-9250 module: high-resolution pinout, datasheet, schema and specs](https://mischianti.org/tdk-invensense-mpu-9250-module-high-resolution-pinout-datasheet-schema-and-specs/) | Pinout, datasheet, specs, how to spot counterfeit modules |
| 2 | [MPU9250 with ESP32 and Arduino: Accelerometer, Magnetometer, and Gyroscope via I2C and SPI](https://mischianti.org/mpu9250-with-esp32-and-arduino-accelerometer-magnetometer-and-gyroscope-via-i2c-and-spi/) ([🇮🇹 Italiano](https://mischianti.org/it/mpu9250-con-esp32-e-arduino-accelerometro-magnetometro-e-giroscopio-tramite-i2c-e-spi/)) | Wiring, I2C and SPI, reading the sensors, DMP |
| 3 | [MPU9250 with ESP32 and Arduino: interrupt and low power mode](https://mischianti.org/mpu9250-with-esp32-and-arduino-interrupt-and-low-power-mode/) | Data-ready and wake-on-motion interrupts, low power, deep sleep |

> [!WARNING]
> **Is your MPU-9250 genuine?** Many modules sold as MPU-9250 actually mount a different chip, most often an **MPU-6500**, which looks almost identical but has no magnetometer.
> The [pinout and specs article](https://mischianti.org/tdk-invensense-mpu-9250-module-high-resolution-pinout-datasheet-schema-and-specs/) includes a **test sketch** that reads the `WHO_AM_I` register to identify the real chip on your board. Run it before you start if the library cannot talk to the sensor or the magnetometer returns no data.

### Related tools and articles

- [3D model viewer: visualize Quaternions and Euler Angles from Serial Data in Real-Time](https://mischianti.org/3d-model-viewer-visualize-quaternions-and-euler-angles-from-serial-data-in-real-time/) – view the orientation computed by the DMP live
- [GY-291 ADXL345 I2C/SPI accelerometer with interrupt for ESP32, ESP8266, STM32 and Arduino](https://mischianti.org/gy-291-adxl345-i2c-spi-accelerometer-with-interrupt-for-esp32-esp8266-stm32-and-arduino/)
- [GY-273 QMC5883L clone HMC5883L magnetometer for Arduino, ESP8266 and ESP32](https://mischianti.org/gy-273-qmc5883l-clone-hmc5883l-magnetometer-for-arduino-esp8266-and-esp32/)

### Browse by topic

[MPU9250](https://mischianti.org/tag/mpu9250/) ·
[9-DOF](https://mischianti.org/tag/9-dof/) ·
[Accelerometer](https://mischianti.org/category/electronic/sensors/accelerometer/) ·
[Gyroscope](https://mischianti.org/category/electronic/sensors/gyroscope/) ·
[Magnetometer](https://mischianti.org/category/electronic/sensors/magnetometer-triaxial-magnetic-field/)

---

## Overview

Advanced Arduino library for the InvenSense MPU-9250 inertial measurement unit (IMU), which enables the sensor's Digital Motion Processing (DMP) features.
Along with configuring and reading from the accelerometer, gyroscope, and magnetometer, this library also supports the chip's DMP features, such as:

- Quaternion calculation
- Pedometer
- Gyroscope calibration
- Tap detection
- Orientation detection

## Repository Contents

| Path                 | Description                                                                                                                                                                                  |
|----------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `/examples`          | Example sketches for the library (`.ino`). Run these from the Arduino IDE.                                                                                                                   |
| `/src`               | Source files for the library (`.cpp`, `.h`).                                                                                                                                                 |
| `/src/util`          | Source and headers for the MPU-9250 driver and DMP configuration, adapted from [InvenSense's downloads page](https://www.invensense.com/developers/software-downloads/#sla_content_45).       |
| `keywords.txt`       | Keywords from this library that will be highlighted in the Arduino IDE.                                                                                                                      |
| `library.properties` | General library properties for the Arduino package manager.                                                                                                                                  |

---

## Getting Started

### Setting Up the MPU-9250

To get started with the MPU-9250, include the library and initialize the sensor:

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

You can configure the full-scale ranges of the gyroscope and accelerometer, the digital low-pass filter (LPF), and the sample rates:

```cpp
// Set gyroscope full-scale range to ±2000 degrees per second (dps)
imu.setGyroFSR(2000);

// Set accelerometer full-scale range to ±2g
imu.setAccelFSR(2);

// Set digital low-pass filter to 5 Hz for smooth data output
imu.setLPF(5);

// Set the sample rate for accelerometer and gyroscope to 10 Hz
imu.setSampleRate(10);

// Set the magnetometer (compass) sample rate to 10 Hz
imu.setCompassSampleRate(10);
```

| Setting               | Method                   | Example value |
|-----------------------|--------------------------|---------------|
| Gyroscope range       | `setGyroFSR()`           | `2000` dps    |
| Accelerometer range   | `setAccelFSR()`          | `2` g         |
| Low-pass filter       | `setLPF()`               | `5` Hz        |
| Accel/Gyro sample rate| `setSampleRate()`        | `10` Hz       |
| Compass sample rate   | `setCompassSampleRate()` | `10` Hz       |

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
    Serial.println("Gyro X: "  + String(imu.calcGyro(imu.gx))  + " dps");
    Serial.println("Mag X: "   + String(imu.calcMag(imu.mx))   + " uT");
  }
}
```

### Using the FIFO for Stable Orientation Data

For applications requiring stable orientation data, you can use the Digital Motion Processor (DMP) with the FIFO buffer to calculate quaternions and Euler angles:

```cpp
void setup() {
  // ... sensor initialization (see above) ...

  // Initialize the DMP to output quaternion data at 10 Hz
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);
}

void loop() {
  // Check if new data is available in the FIFO
  if (imu.fifoAvailable()) {
    // Update the FIFO and calculate quaternion values
    if (imu.dmpUpdateFifo() == INV_SUCCESS) {
      // Compute Euler angles from the quaternion data
      imu.computeEulerAngles();

      // Print Euler angles (Roll, Pitch, Yaw)
      Serial.print("Roll: ");  Serial.println(imu.roll);
      Serial.print("Pitch: "); Serial.println(imu.pitch);
      Serial.print("Yaw: ");   Serial.println(imu.yaw);
    }
  }
}
```

---

## Summary

- **Initialization** – Include the library, initialize the MPU-9250, and enable the required sensors.
- **Configuration** – Customize the gyroscope and accelerometer ranges, set the LPF, and adjust the sample rates.
- **Reading Data** – Read the sensor data directly with `update()`, or use the FIFO buffer for stable orientation measurements.
- **Orientation Tracking** – Use the DMP and FIFO to compute and track orientation with quaternions and Euler angles.

These examples show how to use the MPU-9250 in common scenarios, from basic data acquisition to orientation tracking.

---

## Changelog

| Date       | Version | Notes                                  |
|------------|---------|----------------------------------------|
| 2026-10-05 | 1.0.1   | Fix Wake-on-Motion, implement missing DMP methods, improve I2C error handling, add WebSerial 3D example and update docs |
| 2024-08-19 | 1.0.0   | First commit of the production library |

---

<p align="center">
  Made by <b>Renzo Mischianti</b> · More tutorials, libraries and projects at <a href="https://mischianti.org/"><b>mischianti.org</b></a>
</p>
