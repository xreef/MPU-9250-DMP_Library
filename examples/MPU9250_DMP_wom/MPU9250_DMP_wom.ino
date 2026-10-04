/************************************************************
  MPU9250_Wake_On_Motion
  Wake-on-Motion interrupt sketch for MPU-9250 DMP Arduino Library

  This example demonstrates how to use the Wake-on-Motion (WOM) mode
  to wake the MPU-9250 only when significant motion is detected.
  The sensor runs in low-power accelerometer "cycle" mode (gyroscope
  and magnetometer off): it wakes up at the selected rate, takes one
  accelerometer sample and compares it with the previous one. When
  the difference exceeds the threshold, it pulses the INT pin.

  Optional tuning (call them after mpu_lp_motion_interrupt()):
  - mpu_set_accel_averaging(0..3): 4/8/16/32-sample hardware
    averaging. WOM already enables 32 samples (the least noisy
    setting); lower it only if you need a faster response.
  - mpu_set_wom_mode(0): compare with the sample taken when WOM was
    armed instead of the previous one (detects slow tilts too).

  Renzo Mischianti <info@mischianti.org>
  www.mischianti.org
  https://github.com/xreef/MPU-9250-DMP_Library
*************************************************************/

// Select the correct Serial port based on the platform.
#if defined(SAMD)
  #define SerialPort SerialUSB
#else
  #define SerialPort Serial
#endif

#include <MPU9250-DMP.h>  // Include the MPU9250 DMP library

// GPIO connected to the MPU-9250 INT pin.
// Change it for your board: on ESP32-WROOM modules GPIO 6-11 are used by
// the SPI flash and must not be used (e.g. use GPIO 4 on an ESP32 DevKit).
#define INTERRUPT_PIN 10

// Wake-on-Motion parameters.
#define WOM_THRESHOLD_MG 40  // Motion threshold in mg (4 mg steps, max 1020)
#define WOM_WAKE_RATE_HZ  2  // 1 (= 1.25 Hz), 2 (= 2.5 Hz), 5, 10, 20, 40,
                             // 80, 160, 320 or 640 Hz

// ESP32/ESP8266 interrupt handlers must live in IRAM.
#if defined(ESP32) || defined(ESP8266)
  #define ISR_ATTR IRAM_ATTR
#else
  #define ISR_ATTR
#endif

// Forward declarations. The IRAM attribute goes on this declaration only:
// the definition below inherits it (repeating it makes GCC warn).
void ISR_ATTR imuISR(void);
void readSensorData(void);

// Global flag set by the ISR when motion is detected.
volatile bool imuWoke = false;

// Create an instance of the MPU9250_DMP class.
MPU9250_DMP imu;

// Interrupt Service Routine (ISR) for Wake-on-Motion.
// Called on the falling edge of the interrupt pin.
void imuISR() {
  imuWoke = true;  // Set the flag to indicate motion detection.
}

void setup() {
  // Initialize the interrupt pin with an internal pull-up resistor.
  // The sensor will pull this pin LOW when motion is detected.
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);

  // Begin serial communication at 115200 baud.
  SerialPort.begin(115200);
  delay(2000); // Wait a bit for the serial monitor to open
  SerialPort.println("Starting...");
  SerialPort.println("MPU9250_Wake_On_Motion Example");

  // Initialize the MPU9250 sensor.
  if (imu.begin() != INV_SUCCESS) {
    SerialPort.println("Unable to communicate with MPU-9250");
    SerialPort.println("Check connections, and try again.");
    SerialPort.println();
    while (1);  // Halt the program if initialization fails.
  }

  SerialPort.println("MPU-9250 initialized successfully!");

  // Use only the accelerometer to save power.
  // This powers down the gyroscope and the magnetometer.
  imu.setSensors(INV_XYZ_ACCEL);

  // Configure the interrupt pin to be active-low.
  imu.setIntLevel(INT_ACTIVE_LOW);

  // Disable the I2C bypass to the magnetometer: it is not needed in WOM.
  mpu_set_bypass(0);

  // Attach the interrupt service routine (ISR) before arming, so no
  // falling edge can be missed.
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), imuISR, FALLING);

  // Enable Wake-on-Motion (WOM) low-power mode.
  // Parameters:
  //   Threshold: WOM_THRESHOLD_MG (milli-g) - motion sensitivity threshold.
  //   Duration:  0 (ignored by the MPU-9250, it has no duration filter).
  //   Frequency: WOM_WAKE_RATE_HZ - accelerometer wake-up rate.
  if (mpu_lp_motion_interrupt(WOM_THRESHOLD_MG, 0, WOM_WAKE_RATE_HZ) != INV_SUCCESS) {
    SerialPort.println("IMU setup failed. Please check the installed IMU IC.");
    while (1);  // Halt if WOM setup fails.
  }

  SerialPort.println("MPU9250 configured for Wake-on-Motion.");
}

void loop()
{
  // If a wake-on-motion event is detected, process the sensor data.
  if (imuWoke) {
    // Reset the wake-on-motion flag first, so an event that arrives
    // while we are printing is not lost.
    imuWoke = false;

    SerialPort.println("Motion detected! Reading sensor data...");

    // Read and display sensor data.
    readSensorData();
  }

  // Additional logic can be added here (or put the MCU to sleep).
  delay(10); // Prevents I2C spam on fast microcontrollers like ESP32
}

// Read sensor data if available and display it in a formatted table.
void readSensorData() {
  // Check if new sensor data is available.
  if (imu.dataReady()) {
    // Update the sensor data (accelerometer only).
    // Note: the gyroscope and the magnetometer are disabled to save power
    // during WOM. If you need them, enable them in setup() with:
    // imu.setSensors(INV_XYZ_ACCEL | INV_XYZ_GYRO | INV_XYZ_COMPASS);
    // Be aware this will significantly increase power consumption.
    imu.update(UPDATE_ACCEL);

    // Calculate sensor readings.
    float ax = imu.calcAccel(imu.ax);
    float ay = imu.calcAccel(imu.ay);
    float az = imu.calcAccel(imu.az);

    // Print sensor data in a neat tabular format.
    SerialPort.println("------------------------------------------------");
    SerialPort.println("       Sensor Data (New Motion Detected)        ");
    SerialPort.println("------------------------------------------------");
    SerialPort.print("Accel (g):   X = "); SerialPort.print(ax, 2);
    SerialPort.print(" | Y = "); SerialPort.print(ay, 2);
    SerialPort.print(" | Z = "); SerialPort.println(az, 2);
    SerialPort.println("------------------------------------------------\n");
  }
}
