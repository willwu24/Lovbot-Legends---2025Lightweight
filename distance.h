#ifndef DISTANCE_H
#define DISTANCE_H
#include <Arduino.h>
// #include "Utility.h"
//i2c dist


/*
GY56-----------MINI
VCC ---------- 5V
GND  --------- GND
Pin 5 -------- RC
Pin 4 -------- TD
Pin PS-------GND
*/
#include "Wire.h"

//The Arduino Wire library uses the 7-bit version of the address, so the code example uses 0x70 instead of the 8-bit 0xE0
//The sensors ranging command has a value of 0x51
#define RangeCommand byte(0x51)
//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand1 byte(0xAA)
#define ChangeAddressCommand2 byte(0xA5)
#define ChangModeCommand1  byte(0x5A)
#define ChangModeCommand2  byte(0x55)
#define measure_time_50ms 0
#define measure_time_100ms 1
#define measure_time_200ms 2
#define measure_time_300ms 3
#define DISTANCEMODE_SHORT  1
#define DISTANCEMODE_MEDIUM 2
#define DISTANCEMODE_LONG   3
#define NUM_OF_DISTANCE 4
#define I2C_DISTANCE_SENSOR 0
#define ANALOG_DISTANCE_SENSOR 1
#define DISTANCE_SENSOR I2C_DISTANCE_SENSOR

#if DISTANCE_SENSOR == I2C_DISTANCE_SENSOR
int getDistance(byte which);
void takeRangeReading(byte which);
word requestRange(byte which);
void changeAddress(byte oldAddress, byte newAddress, boolean SevenBitHuh);
void change_mode(byte Address, byte mode, byte  time);
void printDistance();
int getFrontDistanceCM();
int getRightDistanceCM();
int getBackDistanceCM();
int getLeftDistanceCM();


#elif DISTANCE_SENSOR == ANALOG_DISTANCE_SENSOR
int getDistance(int side);

#endif 

void setupDistance();

#endif