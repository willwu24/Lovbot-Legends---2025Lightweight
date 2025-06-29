#include "coordinate.h"

// --- Main Coordinate Function ---
void getCoordinate() {
  long currTime = millis();
  long acceptedInterval = 100;
  posInfo returnPosition;

  int left  = getUltraLeft();
  int right = getUltraRight();
  int back  = getUltraBack();
  int front = getUltraFront();

  posInfo rawPosition = findCO(front, right, back, left);

  // --- X Coordinate Calculation ---
  if (isXBlocked(left, right)) {
    if ((currTime - prevPosition.timeX) < acceptedInterval) {
      returnPosition.posX = prevPosition.posX;
      returnPosition.isXBlocked = false;
    } else {
      returnPosition.isXBlocked = true;
      returnPosition.posX = 0;
    }
  } else {
    returnPosition.posX = rawPosition.posX;
    returnPosition.isXBlocked = false;
    prevPosition.posX = rawPosition.posX;
    prevPosition.timeX = currTime;
  }

  // --- Y Coordinate Calculation ---
  if (isYBlocked(front, back)) {
    if ((currTime - prevPosition.timeY) < acceptedInterval) {
      returnPosition.posY = prevPosition.posY;
      returnPosition.isYBlocked = false;
    } else {
      returnPosition.isYBlocked = true;
      returnPosition.posY = 0;
    }
  } else {
    returnPosition.posY = rawPosition.posY;
    returnPosition.isYBlocked = false;
    prevPosition.posY = rawPosition.posY;
    prevPosition.timeY = currTime;
  }

  currPosition = returnPosition;
}

// --- Coordinate Calculation Methods ---
posInfo findCO(int front, int right, int back, int left) {
  return findCOByCenter(front, right, back, left);
  // Alternative: return findCOByCorner(front, right, back, left);
}

posInfo findCOByCorner(int front, int right, int back, int left) {
  // LEFT BOTTOM = 0
  posInfo currCO;
  int posX = (left < right) ? left : H_SIZE - right;
  int posY = (back < front) ? back : V_SIZE - front;

  currCO.posX = posX - H_SIZE / 2;
  currCO.posY = posY - V_SIZE / 2;

  return currCO;
}

posInfo findCOByCenter(int front, int right, int back, int left) {
  // CENTER FIELD = (0, 0)
  posInfo currCO;
  currCO.posX = (left - right) / 2;
  currCO.posY = (back - front) / 2;
  return currCO;
}

// --- Obstruction Detection ---
bool isXBlocked(int left, int right)  { return (left + right) < thresHoldh; }  // H threshold
bool isYBlocked(int front, int back)  { return (front + back) < thresHoldv; }  // V threshold

// --- Accessor Functions ---
int getX()            { return currPosition.posX; }
int getY()            { return currPosition.posY; }
int getXBlocked()     { return currPosition.isXBlocked; }
int getYBlocked()     { return currPosition.isYBlocked; }
posInfo getCurrPosition() { return currPosition; }
