#include "coordinateMovement.h"

// Speed control
int minSpeed = 20;
int maxSpeed = 40;

// Parking parameters
int parkThresholdDist = 40;
int parkThresholdTime = 200;
bool parking = false;
unsigned long confirmParkTime = -1;

// Movement between coordinates
unsigned long parkedTime = -1;
int waitMoveTime = 3000;
const int NUM_OF_PARKING_AREAS = 4;
int defensePark[2] = {0, 0};
int parkingArea[NUM_OF_PARKING_AREAS][2] = {
  {0, 0}, {0, 35}, {35, 0}, {0, -35}
};
int possibilities[NUM_OF_PARKING_AREAS] = {25, 25, 25, 25};
int currentSpot = 0;
posInfo targetCoordinates;

// Time tracking
unsigned long currTime = millis();

void goToCoordinate(int tarX, int tarY){
  currTime = millis();
  posInfo currCO = getCurrPosition();
  moveInfo currMove;
  posInfo targetCO;
  targetCO.posX = tarX;
  targetCO.posY = tarY;

  if ((getSpeed() < 5 || getDir() == 360) && parking) {
    return;
  }

  if (currCO.isXBlocked || currCO.isYBlocked) {
    parking = false;
    parkedTime = -1;
    currMove = blockedCalculator(currCO, targetCO);
    // Serial.print("Blocked");
  } else {
    parkedTime = -1;
    currMove.dist = distCalculator(currCO, targetCO);
    currMove.dir = isParked(currMove.dist) ? 360 : dirCalculator(currCO, targetCO);
  }
  
  setTarget(0);
  setDir(currMove.dir);
  setSpeed(speedCalculator(currMove.dist));
}

bool isParked(int dist) {
  if (dist < parkThresholdDist) {
    if ((currTime - confirmParkTime) > parkThresholdTime) {
      parking = true;
      return true;
    } else if (confirmParkTime == -1) {
      confirmParkTime = currTime;
    }
  }
  parking = false;
  return false;
}

int speedCalculator(int dist) {
  int speed = minSpeed + (maxSpeed - minSpeed) * abs(dist) * 0.005;
  return constrain(speed, minSpeed, maxSpeed);
}

int distCalculator(posInfo currCO, posInfo targetCO) {
  int dx = targetCO.posX - currCO.posX;
  int dy = targetCO.posY - currCO.posY;
  return sqrt(dx * dx + dy * dy);
}

int dirCalculator(posInfo currCO, posInfo targetCO) {
  int dx = targetCO.posX - currCO.posX;
  int dy = targetCO.posY - currCO.posY;
  int dir = toDegree(atan2(dx, dy));
  return (dir < 0) ? dir + 360 : dir;
}

moveInfo blockedCalculator(posInfo currCO, posInfo targetCO) {
  moveInfo move;
  if (currCO.isXBlocked && !currCO.isYBlocked) {
    move.dir = (targetCO.posY > currCO.posY) ? 0 : 180;
    move.dist = targetCO.posY - currCO.posY;
  } else if (currCO.isYBlocked && !currCO.isXBlocked) {
    move.dir = (targetCO.posX > currCO.posX) ? 90 : 270;
    move.dist = targetCO.posX - currCO.posX;
  } else {
    move.dir = 360;
  }
  return move;
}
