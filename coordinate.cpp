#include "coordinate.h"

void getCoordinate() {
  long currTime = millis();
  long acceptedInterval = 0;
  posInfo returnPosition;
  int left = getUltraLeft();
  int right = getUltraRight();
  int back = getUltraBack();
  int front = getUltraFront();
  posInfo rawPosition = findCO(front, right, back, left);

  if (isXBlocked(left, right)) { // remove prevposition and just keep returnposition?
    if ((currTime - prevPosition.timeX) < acceptedInterval) {
      returnPosition.posX = prevPosition.posX;
      returnPosition.isXBlocked = false;
    }

    else {
      returnPosition.isXBlocked = true;
      returnPosition.posX = 0;
    }

  } else {
    returnPosition.posX = rawPosition.posX;
    returnPosition.isXBlocked = false;
    prevPosition.posX = rawPosition.posX;
    prevPosition.timeX = currTime;
  }

  if (isYBlocked(front, back)) {

    if (currTime - prevPosition.timeY < acceptedInterval) {
      returnPosition.posY = prevPosition.posY;
      returnPosition.isYBlocked = false;
    }

    else {
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

posInfo findCO(int front, int right, int back, int left) {
  return findCOByCenter(front, right, back, left);
  //return findCOByCorner(front, right, back, left);
}
posInfo findCOByCorner(int front, int right, int back, int left) { // LEFT BOTTOM = 0
  posInfo currCO;
  int posX = 0;
  int posY = 0;
  if (left < right) {
    posX = left;
  } else {
    posX = H_SIZE - right;
  }
  if (back < front) {
    posY = back;
  } else {
    posY = V_SIZE - front;
  }
  currCO.posX = posX-H_SIZE/2;
  currCO.posY = posY-V_SIZE/2;
  return currCO;
}
posInfo findCOByCenter(int front, int right, int back,
                       int left) { // MIDDLE FIELD = 0
  posInfo currCO;
  currCO.posY = (back - front) / 2;
  currCO.posX = (left - right) / 2;
  return currCO;
}
bool isXBlocked(int left, int right) { return left + right < 0; }//thresHoldh
bool isYBlocked(int front, int back) { return front + back < 0; }//thresHoldv

int getX() { return currPosition.posX; }
int getY() { return currPosition.posY; }
int getXBlocked() { return currPosition.isXBlocked; }
int getYBlocked() { return currPosition.isYBlocked; }
posInfo getCurrPosition(){ return currPosition;}