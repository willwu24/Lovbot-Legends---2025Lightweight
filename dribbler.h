#ifndef DRIBBLER_H
#define DRIBBLER_H
#include <Arduino.h>
#include "func.h"
#include "led.h"
#include <Servo.h>

void setupDribbler();
void setDribbler(int, int);
bool getDribblerState();
bool getDribblerSpeed();
long getLastLowTime();
void setLastLowTime();

#endif
