#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include <Arduino.h>
#include "func.h"
#include <queue>

// #define TOTAL_GRAYSCALE 7
// #define THRES_TOLERANCE 80


void initSensors();
void calibrateThreshold();
void calibrateLightweightThreshold();
void resetCalibrateThreshold();
void processSensors();
void printSensorsThres();
void printSensorsReading();
void printSensorsWhite();
void printWhiteSensors();
void printMaxThres();
void printMinThres();
void printDiffThres();
int getLightDif(int currentLight, int lastLight);

int getFirstWhiteAngle();
bool getFirstWhite();
bool whiteDetected();
void processWhiteAngle();
int getWhiteAngle();
int getRawWhiteAngle();


void printWhiteHistory();
int getWhiteCountHistory();

void resetWhiteLine();

bool getFlip();
void setFlip(bool a);

int getLastFlip();
void setFirstFlip(bool a);

double getMagnitude();
double getXSum();
double getYSum();
bool getInCorner();

#endif