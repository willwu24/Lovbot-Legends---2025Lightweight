#include "coordinateMovement.h"

// Speed:
int minSpeed = 20;
int maxSpeed = 40;

// Parking flag:
int parkThresholdDist = 0;//20
int parkThresholdTime = 200;
bool parking = false;
unsigned long confirmParkTime = -1;

// Move to other coordinate:
unsigned long parkedTime = -1;
int waitMoveTime = 3000;
const int NUM_OF_PARKING_AREAS = 4;
int defensePark[2] = {0, 0};
int parkingArea[NUM_OF_PARKING_AREAS][2] = {{0, 0}, {0, 35}, {35, 0}, {0, -35}};
// int parkingArea[NUM_OF_PARKING_AREAS][2] = {{135, 70},{30,70}};
int possibilities[NUM_OF_PARKING_AREAS] = {25, 25, 25, 25}; // in random preference, the chance you will get the parking spot

int currentSpot = 0;
posInfo targetCoordinates;

// Coordinates:
unsigned long currTime = millis();

void setupCoordinateMovement(){
  getNextCORandom();
}

void backPositionDefense(){
  currTime = millis();
  posInfo currCO = getCurrPosition();
  moveInfo currMove;
  setTargetCO();
  posInfo targetCO;
  targetCO.posX = defensePark[0];
  targetCO.posY = defensePark[1];
  if ((getSpeed() < 5 || getDir() == 360) && parking == true) {
    // if (changeLocation()) {
    //   parkedTime = -1;
    //   parking = false;
    //   getNextCORandomAvoid();
    // }

    return;
  }
  if (currCO.isXBlocked || currCO.isYBlocked) {//else if (currCO.isXBlocked || currCO.isYBlocked) {
    parking = false;
    parkedTime = -1;
    currMove = blockedCalculator(currCO, targetCO);
  }

  else {
    parkedTime = -1;
    currMove.dist = distCalculator(currCO, targetCO);
    currMove.dir = dirCalculator(currCO, targetCO);
  }
  setDir(currMove.dir);
  setSpeed(speedCalculator(currMove.dist));
  return;
}

void moveToCoordinate() {
  currTime = millis();
  posInfo currCO = getCurrPosition();
  moveInfo currMove;
  setTargetCO();
  posInfo targetCO;
  targetCO.posX = targetCoordinates.posX;
  targetCO.posY = targetCoordinates.posY;
  Serial.print(parking);
  if((getSpeed() < 5 || getDir() == 360) && parking == true) {
    // if (changeLocation()) {
    //   parkedTime = -1;
    //   parking = false;
    //   getNextCORandomAvoid();
    // }

    return;
  }

  else if (currCO.isXBlocked || currCO.isYBlocked) {
    parking = false;
    parkedTime = -1;
    currMove = blockedCalculator(currCO, targetCO);
  }

  else {
    parkedTime = -1;
    currMove.dist = distCalculator(currCO, targetCO);
    if (isParked(currMove.dist)) {
      currMove.dir = 360;
    }

    else {
      currMove.dir = dirCalculator(currCO, targetCO);
    }
  }

  setDir(currMove.dir);
  setSpeed(speedCalculator(currMove.dist));
  setTarget(0);
  return;
}

void cornerPark(){
  int smallSide = -1;
  if(getUltraLeft()<getUltraRight()){
    smallSide = getUltraLeft();
  }
  else{
    smallSide = getUltraRight();
  }
  if(abs(getUltraFront()-75)<10&&abs(smallSide - 30)){
    setDir(STOP);
  }
  else{
    setDir(toDegree(atan2(smallSide - 30,getUltraFront()-75)));
  }
  if(getDir()<0){
    setDir(getDir()+360);
  }
  if(getXBlocked()){
    setDir(180);
  }
  // else if(getUltraFront()<75){
  //   setDir(180);
  // }
  // else{
  //   setDir(0);
  // }
  setSpeed(10+abs(getUltraFront()-75)*0.4);
}

bool isParked(int dist) {
  if (dist < parkThresholdDist) {
    if ((currTime - confirmParkTime) > 200) {
      parking=true;
      return true;
    } else if (confirmParkTime == -1) {
      confirmParkTime = currTime;
    }
  }
  parking=false;
  return false;
}

int speedCalculator(int dist) {
  int sped = minSpeed+(maxSpeed-minSpeed)*abs(dist)*0.005;
  return constrain(sped, minSpeed, maxSpeed);
}

int distCalculator(posInfo currCO, posInfo targetCO) {
  int hDist = targetCO.posX - currCO.posX;
  int vDist = targetCO.posY - currCO.posY;
  int dist = sqrt(hDist * hDist + vDist * vDist);
  return dist;
}

int dirCalculator(posInfo currCO, posInfo targetCO) {
  int hDist = targetCO.posX - currCO.posX;
  int vDist = targetCO.posY - currCO.posY;
  int dir =  toDegree(atan2(hDist, vDist));
  if (dir < 0) {
    dir += 360;
  }
  return dir;
}

moveInfo blockedCalculator(posInfo currCO, posInfo targetCO) {
  moveInfo currMove;
  int dist;
  if (currCO.isXBlocked && !currCO.isYBlocked) {
    //Serial.print("H Blocked");
    dist = targetCO.posY - currCO.posY;
    if (dist > 0) {
      currMove.dir = 0;
    } else {
      currMove.dir = 180;
    }

    currMove.dist = dist;
  }

  else if (currCO.isYBlocked && !currCO.isXBlocked) {
    dist = targetCO.posX - currCO.posX;
    //Serial.print("V Blocked");
    if (dist > 0) {
      currMove.dir = 90;
    } else {
      currMove.dir = 270;
    }
    currMove.dist = dist;
  }

  else {
    currMove.dir = 360;
    //Serial.print("A Blocked");
  }
  return currMove;
}

bool changeLocation() {
  if (parkedTime == -1) {
    parkedTime = currTime;
  } else if (currTime - parkedTime > waitMoveTime) {
    return true;
  }
  return false;
}

void getNextCO() {
  currentSpot = (currentSpot + 1) % NUM_OF_PARKING_AREAS;
  targetCoordinates.posX = parkingArea[currentSpot][0];
  targetCoordinates.posY = parkingArea[currentSpot][1];
}

// void getNextCORandom() {
//   int newSpot = chooseRandom(NUM_OF_PARKING_AREAS, possibilities);
//   if(newSpot!=-1){
//       currentSpot = newSpot;
//   }
// }

// void getNextCORandomAvoid() {
//   int newSpot = chooseRandomBan(NUM_OF_PARKING_AREAS, possibilities, currentSpot);
//   if(newSpot!=-1){
//       currentSpot = newSpot;
//   }
// }

void setNeutralSpot(){
  currentSpot = 1;
}

void setCenter(){
  currentSpot = 0;
}

void setTargetCO() {
  targetCoordinates.posX = parkingArea[0][0];
  targetCoordinates.posY = parkingArea[0][1];
}
