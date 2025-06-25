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
int getUltraFrontCM();
int getUltraLeftCM();
int getUltraBackCM();
int getUltraRightCM();

double getAdjustedRightDistance();

#endif  // DFULTRATRIG_H
