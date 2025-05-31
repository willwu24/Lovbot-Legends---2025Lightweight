#include <EEPROM.h>
#include "Grayscale.h"
#include <PID_v1.h>
#include <queue>

// --- EEPROM Constants ---
const int EEPROM_FLAG_ADDR = 0;
const byte EEPROM_FLAG_VALUE = 123;
const int EEPROM_START_ADDR = 1;

// --- Pin Configuration ---
int readPins[] = {A0, A1};
int readPinsSize = 2;

int selectorPins[] = {38, 39, 40, 41};
int selectorPinsSize = 4;

// --- Sensor and Thresholds ---
const int sensorSize = 32;
int sensorReadings[sensorSize];
int minSensorThres[sensorSize];
int maxSensorThres[sensorSize];
int sensorThres[sensorSize];
bool sensorWhite[sensorSize];
int sensorWhiteHistory[sensorSize];

// --- White Line Detection State ---
bool touchWhite = false;
int whiteCount = 0;
int whiteCountHistory = 0;
bool firstWhite = false;
int firstWhiteIndex = -1;
int firstWhiteAngle = -1;
unsigned long lastFirstWhite = -1;

// --- White Angle Processing ---
int whiteAngle;
int previousWhiteAngle;
int lastAngle;
bool flip = false;
bool firstFlip = true;
bool lastFlip = false;
unsigned long lastWhiteTime = 0;
int previousAngle = -1;

// --- Movement / Position Info ---
double magnitude;
double xDistance = 0;
double yDistance = 0;

// --- Corner Detection ---
int cornerCounter = 0;
bool inCorner = false;
std::queue<int> pastWhites;

// --- Threshold Calibration ---
int calibrationTimer = 0;
const double sensitivity = 0.30;

// --- PID Setup ---
double whiteLineKp = 5.0, whiteLineKi = 0.0, whiteLineKd = 0.0;
double whiteLineSetPoint, whiteLineInput, whiteLineOutput;
PID whiteLinePID(&whiteLineInput, &whiteLineOutput, &whiteLineSetPoint, whiteLineKp, whiteLineKi, whiteLineKd, DIRECT);

// === Initialization ===
void initSensors() {
  for (int i = 0; i < selectorPinsSize; i++) pinMode(selectorPins[i], OUTPUT);
  for (int i = 0; i < readPinsSize; i++) pinMode(readPins[i], INPUT);

  if (!loadThresholdsFromEEPROM()) {
    for (int i = 0; i < sensorSize; i++) {
      maxSensorThres[i] = 0;
      minSensorThres[i] = 999;
      sensorThres[i] = 0;
    }
  }

  whiteLinePID.SetMode(AUTOMATIC);
  whiteLinePID.SetOutputLimits(-50, 50);
  whiteLineSetPoint = 0;
}

// === Calibration ===
void calibrateThreshold() {
  Serial.println("calibrateThreshold");

  for (int i = 0; i < sensorSize / 2; i++) {
    digitalWrite(selectorPins[0], i & 0b0001);
    digitalWrite(selectorPins[1], (i & 0b0010) >> 1);
    digitalWrite(selectorPins[2], (i & 0b0100) >> 2);
    digitalWrite(selectorPins[3], (i & 0b1000) >> 3);

    int reading = analogRead(readPins[0]);
    int reading16 = analogRead(readPins[1]);

    maxSensorThres[i] = max(maxSensorThres[i], reading);
    maxSensorThres[i + 16] = max(maxSensorThres[i + 16], reading16);

    minSensorThres[i] = min(minSensorThres[i], reading);
    minSensorThres[i + 16] = min(minSensorThres[i + 16], reading16);

    sensorThres[i] = minSensorThres[i] + (maxSensorThres[i] - minSensorThres[i]) * sensitivity;
    sensorThres[i + 16] = minSensorThres[i + 16] + (maxSensorThres[i + 16] - minSensorThres[i + 16]) * sensitivity;
  }

  saveThresholdsToEEPROM();
}

void resetCalibrateThreshold() {
  for (int i = 0; i < sensorSize; i++) {
    sensorThres[i] = 0;
  }
}

// === EEPROM Save/Load ===
void saveThresholdsToEEPROM() {
  int addr = EEPROM_START_ADDR;

  for (int i = 0; i < sensorSize; i++) {
    EEPROM.update(addr++, highByte(minSensorThres[i]));
    EEPROM.update(addr++, lowByte(minSensorThres[i]));
  }
  for (int i = 0; i < sensorSize; i++) {
    EEPROM.update(addr++, highByte(maxSensorThres[i]));
    EEPROM.update(addr++, lowByte(maxSensorThres[i]));
  }
  for (int i = 0; i < sensorSize; i++) {
    EEPROM.update(addr++, highByte(sensorThres[i]));
    EEPROM.update(addr++, lowByte(sensorThres[i]));
  }

  EEPROM.update(EEPROM_FLAG_ADDR, EEPROM_FLAG_VALUE);
  Serial.println("Thresholds saved to EEPROM.");
}

bool loadThresholdsFromEEPROM() {
  if (EEPROM.read(EEPROM_FLAG_ADDR) != EEPROM_FLAG_VALUE) {
    Serial.println("EEPROM not initialized, skipping load.");
    return false;
  }

  int addr = EEPROM_START_ADDR;

  for (int i = 0; i < sensorSize; i++) {
    minSensorThres[i] = (EEPROM.read(addr++) << 8) | EEPROM.read(addr++);
  }
  for (int i = 0; i < sensorSize; i++) {
    maxSensorThres[i] = (EEPROM.read(addr++) << 8) | EEPROM.read(addr++);
  }
  for (int i = 0; i < sensorSize; i++) {
    sensorThres[i] = (EEPROM.read(addr++) << 8) | EEPROM.read(addr++);
  }

  Serial.println("Thresholds loaded from EEPROM.");
  return true;
}

// === Sensor Processing ===
void processSensors() {
  int newTouchWhite = 0;
  whiteCount = 0;
  whiteCountHistory = 0;
  firstWhite = false;

  for (int i = 0; i < sensorSize / 2; i++) {
    digitalWrite(selectorPins[0], i & 0b0001);
    digitalWrite(selectorPins[1], (i & 0b0010) >> 1);
    digitalWrite(selectorPins[2], (i & 0b0100) >> 2);
    digitalWrite(selectorPins[3], (i & 0b1000) >> 3);

    sensorReadings[i] = analogRead(readPins[0]);
    sensorReadings[i + 16] = analogRead(readPins[1]);
  }

  for (int i = 0; i < sensorSize; i++) {
    sensorWhite[i] = sensorReadings[i] > sensorThres[i];
    whiteCount += sensorWhite[i];
    if (sensorWhite[i]) sensorWhiteHistory[i] = 1;
    whiteCountHistory += sensorWhiteHistory[i];
    newTouchWhite |= sensorWhite[i];
    if (newTouchWhite && !touchWhite) {
      touchWhite = true;
      firstWhiteIndex = sensorWhite[i] ? i : i + 32;
      firstWhite = true;
    }
  }

  touchWhite = newTouchWhite;
  firstWhiteIndex = touchWhite ? firstWhiteIndex : -1;
  firstWhiteAngle = touchWhite ? ((firstWhiteIndex * (360 / 32)) - 90) % 360 : -1;
  firstWhiteAngle = (firstWhiteAngle + 360) % 360;
}

void processWhiteAngle() {
  double xClusterSum[16] = {0};
  double yClusterSum[16] = {0};
  double clusterAngle[16] = {0};
  double sensorAngle[sensorSize] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
  double xSum = 0;
  double ySum = 0;

  int clusterCount = 0;

  for (int i = 0; i < 32; i++) {
    if (sensorWhite[i]) {
      xClusterSum[clusterCount] += sin(toRadian(360 / sensorSize * i));
      yClusterSum[clusterCount] += cos(toRadian(360 / sensorSize * i));
    } else {
      if (i > 0 && sensorWhite[i - 1]) clusterCount++;
    }
  }
  if (sensorWhite[31]) clusterCount++;
  if (sensorWhite[0] && sensorWhite[31]) {
    xClusterSum[0] += xClusterSum[clusterCount - 1];
    yClusterSum[0] += yClusterSum[clusterCount - 1];
    xClusterSum[clusterCount - 1] = 0;
    yClusterSum[clusterCount - 1] = 0;
    clusterCount--;
  }

  for (int j = 0; j < 16; j++) {
    double len = sqrt(pow(xClusterSum[j], 2) + pow(yClusterSum[j], 2));
    if (len > 0) {
      xClusterSum[j] /= len;
      yClusterSum[j] /= len;
      clusterAngle[j] = atan2(xClusterSum[j], yClusterSum[j]) * (180 / 3.14);
    }
  }

  for (int c = 0; c < clusterCount; c++) {
    xSum += xClusterSum[c];
    ySum += yClusterSum[c];
  }

  double rawAngle = atan2(xSum, ySum) * 180 / 3.14;
  rawAngle = ((int(rawAngle) + 360) % 360);
  int finalAngle = rawAngle;

  if (!whiteDetected()) {
    firstFlip = true;
  } else {
    if (firstFlip) {
      firstFlip = false;
    } else if (whiteDetected() && abs(getLightDif(lastAngle, rawAngle)) > 120) {
      flip = !flip;
      lastWhiteTime = millis();
    }
  }

  if (!whiteDetected() && millis() - lastWhiteTime > 3000) {
    flip = 0;
  }

  whiteAngle = ((finalAngle + 180 * flip) + 270) % 360;
  if (abs(xSum) < 0.01 && abs(ySum) < 0.01) whiteAngle = previousWhiteAngle;

  magnitude = sqrt(pow(xSum, 2) + pow(ySum, 2));
  if (flip) magnitude = -magnitude;

  previousWhiteAngle = whiteAngle;
  lastAngle = rawAngle;

  whiteLineInput = abs(xSum) + abs(ySum);
  whiteLinePID.Compute();
  xDistance = xSum;
  yDistance = ySum;
}

// --- Utility ---
void resetWhiteLine() {
  for (int i = 0; i < sensorSize; i++) {
    sensorWhiteHistory[i] = 0;
  }
  whiteCountHistory = 0;
}

int getLightDif(int currentLight, int lastLight) {
  int output = 0;
  int current = currentLight;
  int target = lastLight;

  if (target < 180) {
    if (current > target + 180) {
      output = -360 + current - target;
    } else {
      output = current - target;
    }
  } else {
    if (current > target - 180) {
      output = current - target;
    } else {
      output = 360 - target + current;
    }
  }
  return output;
}

int getFirstWhiteAngle() { return firstWhiteAngle; }
bool getFirstWhite()     { return firstWhite; }
bool whiteDetected()     { return touchWhite; }
int getWhiteCountHistory() { return whiteCountHistory; }
int getWhiteAngle()      { return whiteAngle; }
int getRawWhiteAngle()   { return (whiteAngle + 180 * flip) % 360; }
bool getFlip()           { return flip; }
double getMagnitude()    { return magnitude; }
double getXSum()         { return xDistance; }
double getYSum()         { return yDistance; }
bool getInCorner()       { return inCorner; }
