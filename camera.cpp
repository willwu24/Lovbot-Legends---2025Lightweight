#include "camera.h"

// State tracking for last seen blobs
int lastYellow = 0, lastYellowAngle, lastYellowDistance;
int lastBlue = 0, lastBlueAngle, lastBlueDistance;

// Buffer arrays
byte tempData[10];
byte data[10];

void setupCamera() {
  Serial7.begin(57600);
}

void getCameraData() {
  const unsigned long timeoutThreshold = 500;
  static unsigned long lastReceivedDataTime = 0;

  if (Serial7.available() >= 12) {
    byte header = Serial7.read();
    if (header == 0x99) {
      byte checksum = 0;
      for (int i = 0; i < 10; i++) {
        tempData[i] = Serial7.read();
        checksum += tempData[i];
      }
      byte checksum_received = Serial7.read();

      if (checksum_received == (checksum % 256)) {
        memcpy(data, tempData, sizeof(data));
        lastReceivedDataTime = millis();
      } else {
        Serial.println("Checksum error!");
      }
    }
  }

  if (millis() - lastReceivedDataTime > timeoutThreshold) {
    memset(data, -1, sizeof(data));
  }
}

// === Yellow Blob Functions ===
int getYellowAngle() {
  if (data[2] == 1) {
    lastYellow = millis();
    return lastYellowAngle = data[0] * 2;
  }
  return (millis() - lastYellow < 200) ? lastYellowAngle : -1;
}

int getYellowDistance() {
  if (data[2] == 1) {
    lastYellow = millis();
    return lastYellowDistance = data[1];
  }
  return (millis() - lastYellow < 200) ? lastYellowDistance : -1;
}

bool yellowDetected() {
  if (data[2] == 1) {
    lastYellow = millis();
    return true;
  }
  return (millis() - lastYellow < 200);
}

int getYellowLeftEdgeAngle() {
  return (data[2] == 1) ? data[3] * 2 : -1;
}

int getYellowRightEdgeAngle() {
  return (data[2] == 1) ? data[4] * 2 : -1;
}

// === Blue Blob Functions ===
int getBlueAngle() {
  if (data[7] == 1) {
    lastBlue = millis();
    return lastBlueAngle = data[5] * 2;
  }
  return (millis() - lastBlue < 200) ? lastBlueAngle : -1;
}

int getBlueDistance() {
  if (data[7] == 1) {
    lastBlue = millis();
    return lastBlueDistance = data[6];
  }
  return (millis() - lastBlue < 200) ? lastBlueDistance : -1;
}

bool blueDetected() {
  if (data[7] == 1) {
    lastBlue = millis();
    return true;
  }
  return (millis() - lastBlue < 200);
  // return false;
}

int getBlueLeftEdgeAngle() {
  return (data[7] == 1) ? data[8] * 2 : -1;
}

int getBlueRightEdgeAngle() {
  return (data[7] == 1) ? data[9] * 2 : -1;
}

// === Team Context Wrappers ===
int getHomeAngle()             { return getAttackMode() == 0 ? getYellowAngle() : getBlueAngle(); }
int getHomeDistance()          { return getAttackMode() == 0 ? getYellowDistance() : getBlueDistance(); }
bool homeDetected()           { return getAttackMode() == 0 ? yellowDetected() : blueDetected(); }
int getOpposingAngle()        { return getAttackMode() == 1 ? getYellowAngle() : getBlueAngle(); }
int getOpposingDistance()     { return getAttackMode() == 1 ? getYellowDistance() : getBlueDistance(); }
bool opposingDetected()       { return getAttackMode() == 1 ? yellowDetected() : blueDetected(); }
int getHomeLeftEdgeAngle()    { return getAttackMode() == 0 ? getYellowLeftEdgeAngle() : getBlueLeftEdgeAngle(); }
int getHomeRightEdgeAngle()   { return getAttackMode() == 0 ? getYellowRightEdgeAngle() : getBlueRightEdgeAngle(); }
int getOpposingLeftEdgeAngle(){ return getAttackMode() == 1 ? getYellowLeftEdgeAngle() : getBlueLeftEdgeAngle(); }
int getOpposingRightEdgeAngle(){ return getAttackMode() == 1 ? getYellowRightEdgeAngle() : getBlueRightEdgeAngle(); }
