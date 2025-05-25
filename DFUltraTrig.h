#ifndef DFULTRATRIG_H
#define DFULTRATRIG_H

#include <Arduino.h>
#include "func.h"

// === Ultrasonic Trigger Processing ===
void processDFUltraTrig();

// === Distance Accessors ===
int getUltra(int index);
int getUltraFront();
int getUltraLeft();
int getUltraBack();
int getUltraRight();

#endif  // DFULTRATRIG_H
