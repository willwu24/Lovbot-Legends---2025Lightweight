#ifndef COORDINATE_H
#define COORDINATE_H

#include "player.h"
#include "func.h"

// --- Coordinate Calculation ---
void getCoordinate();
posInfo findCO(int front, int right, int back, int left);
posInfo findCOByCorner(int front, int right, int back, int left);
posInfo findCOByCenter(int front, int right, int back, int left);

// --- Obstruction Checks ---
bool isXBlocked(int left, int right);
bool isYBlocked(int front, int back);

// --- Accessors ---
int getX();
int getY();
int getXBlocked();
int getYBlocked();
posInfo getCurrPosition();

#endif
