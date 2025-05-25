#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "compass.h"
#include "camera.h"
#include "DFUltraTrig.h"

void displayDistance();
void displayCoordinate();
void displayCompass();
void displayCamera();

#endif
