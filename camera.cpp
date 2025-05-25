#include "camera.h"

int lastYellow = 0;
int lastYellowAngle;
int lastYellowDistance;

int lastBlue = 0;
int lastBlueAngle;
int lastBlueDistance;

byte tempData[10];
byte data[10];

void setupCamera(){
    Serial7.begin(57600);
}

void getCameraData() {
  const unsigned long timeoutThreshold = 500; // Set the timeout threshold in milliseconds
  static unsigned long lastReceivedDataTime = 0; // Record time of last valid data reception
  
  if (Serial7.available() >= 12) {
    byte header = Serial7.read();
    if (header == 0x99) { // if the header is correct
      byte checksum = 0;
      
      // read the data bytes into tempData and calculate checksum
      for (int i = 0; i < 10; i++) {
        tempData[i] = Serial7.read();
        checksum += tempData[i];
      }
      
      // read checksum byte from stream
      byte checksum_received = Serial7.read();
      
      // validate checksum
      if (checksum_received == (checksum % 256)) {
        for (int i = 0; i < 10; i++) {
          data[i] = tempData[i];
        }
        lastReceivedDataTime = millis();
      }
      else {
        Serial.println("Checksum error!");
      }
    }
  }

  // If no valid data has been received within the timeout threshold, mark all data as invalid (-1)
  if (millis() - lastReceivedDataTime > timeoutThreshold) {
    for (int i = 0; i < 10; i++) {
      data[i] = -1;
    }
  }
}

// Functions for Yellow Blob
int getYellowAngle(){
  if(data[2] == 1) {
    lastYellow = millis();
    lastYellowAngle = data[0] * 2;
    return data[0] * 2;
  }
  else {
    if(millis() - lastYellow < 200) {
      return lastYellowAngle;
    }
    else {
      return -1;
    }
  }
}

int getYellowDistance(){
  if(data[2] == 1) {
    lastYellow = millis();
    lastYellowDistance = data[1];
    return data[1];
  }
  else {
    if(millis() - lastYellow < 200) {
      return lastYellowDistance;
    }
    else {
      return -1;
    }
  }
}

bool yellowDetected(){
  if(data[2] == 1) {
    lastYellow = millis();
    return true;
  }
  else {
    if(millis() - lastYellow < 200) {
      return true;
    }
    else {
      return false;
    }
  }
}

// New functions to return yellow blob edge angles (left and right)
int getYellowLeftEdgeAngle(){
  if(data[2] == 1) {
    return data[3] * 2;
  }
  return -1;
}

int getYellowRightEdgeAngle(){
  if(data[2] == 1) {
    return data[4] * 2;
  }
  return -1;
}

// Functions for Blue Blob
int getBlueAngle(){
  if(data[7] == 1) {
    lastBlue = millis();
    lastBlueAngle = data[5] * 2;
    return data[5] * 2;
  }
  else {
    if(millis() - lastBlue < 200) {
      return lastBlueAngle;
    }
    else {
      return -1;
    }
  }
}

int getBlueDistance(){
  if(data[7] == 1) {
    lastBlue = millis();
    lastBlueDistance = data[6];
    return data[6];
  }
  else {
    if(millis() - lastBlue < 200) {
      return lastBlueDistance;
    }
    else {
      return -1;
    }
  }
}

bool blueDetected(){
  if(data[7] == 1) {
    lastBlue = millis();
    return true;
  }
  else {
    if(millis() - lastBlue < 200) {
      return true;
    }
    else {
      return false;
    }
  }
}

// New functions to return blue blob edge angles (left and right)
int getBlueLeftEdgeAngle(){
  if(data[7] == 1) {
    return data[8] * 2;
  }
  return -1;
}

int getBlueRightEdgeAngle(){
  if(data[7] == 1) {
    return data[9] * 2;
  }
  return -1;
}

// Helper functions to determine which target is "home" (attack) versus "opposing"
// These functions use getAttackMode() which is assumed to be defined elsewhere.

int getHomeAngle(){
  if(getAttackMode() == 0){
    return getYellowAngle();
  }
  else{
    return getBlueAngle();
  }
}

int getHomeDistance(){
  if(getAttackMode() == 0){
    return getYellowDistance();
  }
  else{
    return getBlueDistance();
  }
}

bool homeDetected(){
  if(getAttackMode() == 0){
    return yellowDetected();
  }
  else{
    return blueDetected();
  }
}

int getOpposingAngle(){
  if(getAttackMode() == 1){
    return getYellowAngle();
  }
  else{
    return getBlueAngle();
  }
}

int getOpposingDistance(){
  if(getAttackMode() == 1){
    return getYellowDistance();
  }
  else{
    return getBlueDistance();
  }
}

bool opposingDetected(){
  if(getAttackMode() == 1){
    return yellowDetected();
  }
  else{
    return blueDetected();
  }
}

// New functions to get the left and right edge angles for the home (attack) target
int getHomeLeftEdgeAngle(){
  if(getAttackMode() == 0){
    return getYellowLeftEdgeAngle();
  }
  else{
    return getBlueLeftEdgeAngle();
  }
}

int getHomeRightEdgeAngle(){
  if(getAttackMode() == 0){
    return getYellowRightEdgeAngle();
  }
  else{
    return getBlueRightEdgeAngle();
  }
}

// New functions to get the left and right edge angles for the opposing target
int getOpposingLeftEdgeAngle(){
  if(getAttackMode() == 1){
    return getYellowLeftEdgeAngle();
  }
  else{
    return getBlueLeftEdgeAngle();
  }
}

int getOpposingRightEdgeAngle(){
  if(getAttackMode() == 1){
    return getYellowRightEdgeAngle();
  }
  else{
    return getBlueRightEdgeAngle();
  }
}
