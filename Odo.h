#ifndef ODO_H
#define ODO_H

#include <Arduino.h>
#include <SparkFun_Qwiic_OTOS_Arduino_Library.h> // Required for QwiicOTOS
#include "func.h"

// Setup OTOS and calibrate
void odoSetup();
void calibrateOdo();


// Read position and update internal state
void odoUpdate();

// Print position to Serial
void printOdo();

// Accessors for current odometry
int getOdoX();
int getOdoY();
int getOdoHeading();

//Compass
int getCompass();


#endif
