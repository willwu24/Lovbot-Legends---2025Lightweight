#ifndef CAMERA_H
#define CAMERA_H
#include <Arduino.h>
#include <Wire.h>
#include "func.h"

// Setup and Data Retrieval
void setupCamera();
void getCameraData();

// Yellow Blob Functions
int getYellowAngle();
int getYellowDistance();
bool yellowDetected();
int getYellowLeftEdgeAngle();
int getYellowRightEdgeAngle();

// Blue Blob Functions
int getBlueAngle();
int getBlueDistance();
bool blueDetected();
int getBlueLeftEdgeAngle();
int getBlueRightEdgeAngle();

// Home (Attack) and Opposing Target Functions
bool homeDetected();
bool opposingDetected();
int getHomeAngle();
int getHomeDistance();
int getOpposingAngle();
int getOpposingDistance();
int getHomeLeftEdgeAngle();
int getHomeRightEdgeAngle();
int getOpposingLeftEdgeAngle();
int getOpposingRightEdgeAngle();

#endif
