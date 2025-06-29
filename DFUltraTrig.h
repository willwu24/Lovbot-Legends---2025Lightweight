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

// double getAdjustedRightDistance();

int trimmedMean5(const int *a, int n);
int getUltraFrontSmooth();
int getUltraLeftSmooth();
int getUltraBackSmooth();
int getUltraRightSmooth();



#endif  // DFULTRATRIG_H
