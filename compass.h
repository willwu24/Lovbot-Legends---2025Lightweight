#ifndef COMPASS_H
#define COMPASS_H

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include "func.h"

// --- BNO055 Configuration Constants ---
#define RST              11    // BNO RST pin number
#define BNO_ADDR         0x28  // I2C address of the BNO055
#define PAGE_ID          0x07  // Register for selecting register page
#define ACC_DATA_X_LSB   0x08  // Page 0: Acceleration Data X LSB
#define OPR_MODE         0x3D  // Page 0: Operation Mode register
#define ACC_CONFIG       0x08  // Page 1: Accelerometer Configuration
#define MODE_AMG         0x07  // Non-fusion mode (Accel + Gyro + Mag)

// --- Function Declarations ---
void setupCompass();
int getCompassRawValue();
int getCompass();
int getCompassDif(int target, int currentValue);
void bno_write(uint8_t i2c_addr, uint8_t reg, uint8_t data);

#endif
