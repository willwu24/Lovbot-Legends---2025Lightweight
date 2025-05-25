#include "coordinate.h"
int odoShiftX = -1;
int odoShiftY = -1;

void getCoordinate() {
  long currTime = millis();
  long acceptedInterval = 10000000;
  posInfo returnPosition;
  int left = getUltraLeft();
  int right = getUltraRight();
  int back = getUltraBack();
  int front = getUltraFront();
  posInfo rawPosition = findCO(front, right, back, left);
  int unblocked = 0;
  // Serial.print("odoShiftY: ");
  // Serial.print(odoShiftY);
  if (isXBlocked(left, right)) { // remove prevposition and just keep returnposition?// || getLeftDistanceCM()<40 || getRightDistanceCM()<40
    //Serial.print("xblocked");
    if ((currTime - prevPosition.timeX) < acceptedInterval) {
      //setLEDState(0,1);
      //Serial.print("use odo");
      // returnPosition.posX = prevPosition.posX;
      returnPosition.isXBlocked = true;//should be false
      returnPosition.posX = odoShiftX-getOdoX();

    }

    else {
      //setLEDState(0,1);
      returnPosition.isXBlocked = true;
      returnPosition.posX = 0;
    }

  } else {
    //setLEDState(0,0);
    unblocked+=1;
    odoShiftX = rawPosition.posX+getOdoX();
    returnPosition.posX = rawPosition.posX;
    returnPosition.isXBlocked = false;
    prevPosition.posX = rawPosition.posX;
    prevPosition.timeX = currTime;
  }

  if (isYBlocked(front, back)) {//||getBackDistanceCM()<40 || getFrontDistanceCM()<40
    //Serial.print("yblocked");
    if (currTime - prevPosition.timeY < acceptedInterval) {
     // Serial.print("use odo");
      //setLEDState(0,1);
      //returnPosition.posY = prevPosition.posY;
      returnPosition.isYBlocked = true;
      returnPosition.posY = odoShiftY-getOdoY();
    }

    else {
      //setLEDState(0,1);
      returnPosition.isYBlocked = true;
      returnPosition.posY = 0;
    }

  } else {
    //setLEDState(0,0);
    unblocked+=1;
    odoShiftY = rawPosition.posY+getOdoY();
    returnPosition.posY = rawPosition.posY;
    returnPosition.isYBlocked = false;
    prevPosition.posY = rawPosition.posY;
    prevPosition.timeY = currTime;
  }
  if(unblocked==2){
    //setLEDState(0,0);
  }

  currPosition = returnPosition;
}

posInfo findCO(int front, int right, int back, int left) {
  return findCOBySmallest(front, right, back, left);
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

posInfo findCOBySmallest(int front, int right, int back, int left){//middle is 0,0
  posInfo currCO;
  if (left<right){
    currCO.posX = left-80;
  }
  else{
    currCO.posX = 80-right;
  }
  if (back<front){
    currCO.posY = back-90;
  }
  else{
    currCO.posY = 90-front;
  }
  return currCO;
}

bool isXBlocked(int left, int right) { return min(left, 80) + min(right,80) < 130; }//130
bool isYBlocked(int front, int back) { return front + back < 140; }//make a thresh for vertical

int getX() { return currPosition.posX; }
int getY() { return currPosition.posY; }
// int getX(){return min(getUltraLeft(), getUltraRight());}
// int getY(){return min(getUltraFront(),getUltraBack());}


int getXBlocked() { return currPosition.isXBlocked; }
int getYBlocked() { return currPosition.isYBlocked; }
void setOdoShiftX(int a){
  odoShiftX=a;
}
posInfo getCurrPosition(){ return currPosition;}