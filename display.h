#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "compass.h"
#include "camera.h"
#include "DFUltraTrig.h"

// Display sensor values to Serial Monitor
void displayDistance();
void displayCompass();
void displayCamera();

#endif // DISPLAY_H
