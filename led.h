#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "func.h"

void setLED();
void setLEDState(int,int);
void clearLEDState();
void setLEDAll(int);
void checkLED();

#endif
