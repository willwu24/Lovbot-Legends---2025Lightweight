#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <Wire.h>
#include "func.h"

// --- Initialization ---
void setupCamera();
void getCameraData();

// --- Yellow Blob Detection ---
int  getYellowAngle();
int  getYellowDistance();
bool yellowDetected();
int  getYellowLeftEdgeAngle();
int  getYellowRightEdgeAngle();

// --- Blue Blob Detection ---
int  getBlueAngle();
int  getBlueDistance();
bool blueDetected();
int  getBlueLeftEdgeAngle();
int  getBlueRightEdgeAngle();

// --- Home (Attack) Target ---
bool homeDetected();
int  getHomeAngle();
int  getHomeDistance();
int  getHomeLeftEdgeAngle();
int  getHomeRightEdgeAngle();

// --- Opposing Target ---
bool opposingDetected();
int  getOpposingAngle();
int  getOpposingDistance();
int  getOpposingLeftEdgeAngle();
int  getOpposingRightEdgeAngle();

#endif  // CAMERA_H
