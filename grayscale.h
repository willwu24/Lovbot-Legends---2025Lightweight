#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include <Arduino.h>
#include <queue>
#include "func.h"

// ------------------------------
// Initialization & Calibration
// ------------------------------
void initSensors();
void calibrateThreshold();
void resetCalibrateThreshold();

// ------------------------------
// Sensor Data Processing
// ------------------------------
void processSensors();
void processWhiteAngle();
int getLightDif(int currentLight, int lastLight);

// ------------------------------
// Threshold & Reading Utilities
// ------------------------------
void printSensorsThres();
void printSensorsReading();
void printSensorsWhite();
void printWhiteSensors();
void printMaxThres();
void printMinThres();
void printDiffThres();

// ------------------------------
// White Line Detection
// ------------------------------
int getFirstWhiteAngle();
bool getFirstWhite();
bool whiteDetected();
void resetWhiteLine();
void printWhiteHistory();
int getWhiteCountHistory();

// ------------------------------
// Angle & Direction
// ------------------------------
int getWhiteAngle();
int getRawWhiteAngle();
bool getFlip();
void setFlip(bool a);
int getLastFlip();        // Not implemented in provided .cpp code
void setFirstFlip(bool a);

// ------------------------------
// White Line Positioning
// ------------------------------
double getMagnitude();
double getXSum();
double getYSum();
bool getInCorner();

#endif // GRAYSCALE_H
