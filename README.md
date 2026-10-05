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
- [Examples](#examples)
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

## Examples

Every sketch is in the [`examples`](examples) folder and can be opened from **File > Examples > MPU9250-DMP** in the Arduino IDE.

| Example | What it does | Key API | Notes |
|---------|--------------|---------|-------|
| [`MPU9250_Basic`](examples/MPU9250_Basic) | Polls accelerometer, gyroscope, magnetometer and temperature and prints them on the serial monitor. | `setSensors()`, `setGyroFSR()`, `setAccelFSR()`, `setLPF()`, `setSampleRate()`, `setCompassSampleRate()`, `update()`, `calcAccel()`, `calcGyro()`, `calcMag()` | Start here: it verifies wiring and I2C communication. 10 Hz sample rate, gyro 2000 dps, accel +/-2 g, LPF 5 Hz. |
| [`MPU9250_Basic_Interrupt`](examples/MPU9250_Basic_Interrupt) | Reads accelerometer, gyroscope and magnetometer only when the INT pin signals that new data is ready. | `enableInterrupt()`, `setIntLevel(INT_ACTIVE_LOW)`, `setIntLatched(INT_LATCHED)`, `update()` | **Data-ready interrupt, not a motion interrupt**: the pin fires at the sample rate (4 Hz) even when the sensor is still. For motion use `DMP_wom`. |
| [`MPU9250_DMP_wom`](examples/MPU9250_DMP_wom) | Low-power **Wake-on-Motion**: gyroscope and magnetometer are powered down, the accelerometer wakes at 2.5 Hz and pulses INT when the motion exceeds 40 mg. | `setSensors(INV_XYZ_ACCEL)`, `setIntLevel()`, `mpu_set_bypass(0)`, `mpu_lp_motion_interrupt()`, `attachInterrupt()`, `dataReady()` | Threshold (4 mg/LSB) and wake-up rate are `#define`s at the top of the sketch. Optional accelerometer averaging. |
| [`MPU9250_FIFO_Basic`](examples/MPU9250_FIFO_Basic) | Reads accelerometer and gyroscope samples from the hardware FIFO instead of polling the registers. | `setSampleRate(100)`, `configureFifo()`, `fifoAvailable()`, `updateFifo()` | Gyroscope and accelerometer are buffered at 100 Hz; the magnetometer cannot be stored in the FIFO by this library. |
| [`MPU9250_DMP_Quaternion`](examples/MPU9250_DMP_Quaternion) | Uses the DMP to compute the 6-axis quaternion and prints it together with pitch, roll and yaw. | `dmpBegin(DMP_FEATURE_6X_LP_QUAT \| DMP_FEATURE_GYRO_CAL, 10)`, `dmpUpdateFifo()`, `computeEulerAngles()` | Roll and pitch are referenced to gravity; yaw is relative to the start-up heading and drifts slowly. |
| [`MPU9250_DMP_Orientation`](examples/MPU9250_DMP_Orientation) | Detects the orientation of the board (Android-style portrait/landscape) and reports it when it changes. | `dmpBegin(DMP_FEATURE_ANDROID_ORIENT)`, `dmpSetOrientation()`, `dmpGetOrientation()` | The `orientationMatrix` in the sketch maps the sensor axes to the board axes. |
| [`MPU9250_DMP_Pedometer`](examples/MPU9250_DMP_Pedometer) | Counts steps with the DMP pedometer and prints the step count and walking time. | `dmpBegin(DMP_FEATURE_PEDOMETER)`, `dmpSetPedometerSteps()`, `dmpSetPedometerTime()`, `dmpGetPedometerSteps()`, `dmpGetPedometerTime()` | Shake the board up and down at stepping speed; the DMP needs a few consecutive steps (typically 5-7) before it starts counting. |
| [`MPU9250_DMP_Tap`](examples/MPU9250_DMP_Tap) | Detects single and double taps with the DMP, here on the Z axis, and prints the tap direction and count. | `dmpBegin(DMP_FEATURE_TAP, 10)`, `dmpSetTap()`, `tapAvailable()`, `getTapDir()`, `getTapCount()` | Try to reach the maximum count of 8 taps. |
| [`MPU9250_DMP_Gyro_Cal`](examples/MPU9250_DMP_Gyro_Cal) | Lets the DMP calibrate the gyroscope and prints the calibrated gyro values. | `dmpBegin(DMP_FEATURE_GYRO_CAL \| DMP_FEATURE_SEND_CAL_GYRO, 10)`, `dmpUpdateFifo()` | Keep the board still: after about 8 seconds without motion the DMP computes the gyro biases and subtracts them. |
| [`MPU9250_WebSerial_3d`](examples/MPU9250_WebSerial_3d) | Streams quaternion and Euler angles in the format of the **3D Model Viewer**. **[Test it live in your browser](https://mischianti.org/3d-model-viewer-visualize-quaternions-and-euler-angles-from-serial-data-in-real-time/)**. | `dmpBegin(DMP_FEATURE_6X_LP_QUAT \| DMP_FEATURE_GYRO_CAL, 10)`, `dmpUpdateFifo()`, `computeEulerAngles()` | Sends `Orientation: heading, pitch, roll` and `Quaternion: w, x, y, z` at 115200 baud. The DMP quaternion is 6-axis: heading is relative to the start-up position. |

> [!TIP]
> **Try the 3D example in your browser.** Upload `MPU9250_WebSerial_3d`, close the Arduino Serial Monitor so the port is free, then open the [3D Model Viewer page on mischianti.org](https://mischianti.org/3d-model-viewer-visualize-quaternions-and-euler-angles-from-serial-data-in-real-time/) and connect to the board's serial port (115200 baud) with the Web Serial API (Chrome or Edge).

<p align="center">
  <img src="resources/mpu-webserial-3d.jpg" width="420" alt="MPU sensor on a breadboard moving the 3D model in the mischianti.org 3D Model Viewer through Web Serial"><br>
  <em>The MPU sensor driving the 3D Model Viewer in real time through the Web Serial API.</em>
</p>

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
