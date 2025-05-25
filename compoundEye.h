#ifndef COMPOUNDEYE_H
#define COMPOUNDEYE_H

#include <Arduino.h>
#include "func.h"

// --- Thresholds ---
#define BALL_THRES      40  // Minimum eye signal to detect the ball

// --- Analog Eye Pins ---
#define EYE_PORT_PIN    A10 // Pin to read which eye segment is active
#define EYE_VALUE_PIN   A11 // Pin to read signal strength

// --- Setup ---
void eyeSetup(); 

// --- Eye Readings ---
int getEyePort();            // Returns active eye port index (0–17)
int getEyeValue();           // Returns signal value from the active port

// --- Ball Detection Angle ---
int getEyeAngle();           // Returns crude angle to ball (0–360)
int getEyeAngleSmooth();     // Returns interpolated angle to ball (0–360)

#endif
