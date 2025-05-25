#ifndef DRIBBLER_H
#define DRIBBLER_H

#include <Arduino.h>
#include <Servo.h>
#include "func.h"
#include "led.h"

// Dribbler control functions
void setupDribbler();
void setDribbler(int speed, int state);

// State retrieval functions
bool getDribblerState();
bool getDribblerSpeed();

// Timing functions
long getLastLowTime();
void setLastLowTime();

#endif // DRIBBLER_H
