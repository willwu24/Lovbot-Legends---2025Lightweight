// #ifndef COMPASS_H
// #define COMPASS_H

// #include "func.h"
// #include <Wire.h>
// #include <Adafruit_BNO055.h>
// #include <Adafruit_Sensor.h>
// #define RST             11    // BNO RST pin number
// #define BNO_ADDR        0x28  // I2C address of first BNO
// #define PAGE_ID         0x07  // BNO register: page select
// #define ACC_DATA_X_LSB  0x08  // BNO page 0 register: Acceleration Data X LSB
// #define OPR_MODE        0x3D  // BNO page 0 register: Operation Mode <3:0>
// #define ACC_CONFIG      0x08  // BNO page 1 register: Accelerometer Config
// #define MODE_AMG        0x07  // non-fusion mode with accel/gyro/mag

// void setupCompass();
// int getCompassRawValue();
// int getCompass();
// int getComapssDif(int target, int currentValue);
// void bno_write(uint8_t i2c_addr, uint8_t reg, uint8_t data);
// // int getRawCompass();
// // int getJMCompass();
// // void setCompassMax(int max);
// // void setCompassType(int type);
// // int getCompassMax();

// #endif
