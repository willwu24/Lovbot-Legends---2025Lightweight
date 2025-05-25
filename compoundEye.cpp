#include "compoundEye.h"

const int eyeNum = 18;

// Thresholds for mapping analog port values to eye sectors (0–17)
int eyePortThres[eyeNum] = {
  32, 63, 94, 125, 157, 188, 220, 254, 285,
  317, 338, 368, 398, 411, 442, 474, 502, 533
};

// --- Setup ---
void eyeSetup() {
  pinMode(EYE_PORT_PIN, INPUT);
  pinMode(EYE_VALUE_PIN, INPUT);
}

// --- Eye Sector Indexing ---
int getEyePort() {
  int portValue = analogRead(EYE_PORT_PIN);

  for (int i = 0; i < eyeNum - 1; i++) {
    if (portValue < (eyePortThres[i] + eyePortThres[i + 1]) / 2) {
      return i;
    }
  }

  return eyeNum - 1;
}

// --- Eye Signal Value ---
int getEyeValue() {
  return analogRead(EYE_VALUE_PIN);
}

// --- Crude Angle Estimation with Front Sensor Correction ---
int getEyeAngle() {
  int port = getEyePort();
  int degree = port * (360.0 / eyeNum);

  int frontAngle = analogRead(A3);  // Front IR sensor
  if (frontAngle > 60 && frontAngle < 180) {
    if (frontAngle < 100) {
      degree = 10;
    } else if (frontAngle < 130) {
      degree = 0;
    } else {
      degree = 350;
    }
  }

  return degree;
}

// --- Smooth Angle Estimation Using Interpolation ---
int getEyeAngleSmooth() {
  int portValue = analogRead(EYE_PORT_PIN);
  float angle = 0.0;

  for (int i = 1; i < eyeNum; i++) {
    if (portValue < eyePortThres[i]) {
      float fraction = float(portValue - eyePortThres[i - 1]) /
                       (eyePortThres[i] - eyePortThres[i - 1]);
      angle = (i - 1 + fraction) * (360.0 / eyeNum);
      return (int)angle;
    }
  }

  // Handle wraparound case (past last threshold)
  float fraction = float(portValue - eyePortThres[eyeNum - 1]) /
                   (360.0 - eyePortThres[eyeNum - 1]);
  angle = (eyeNum - 1 + fraction) * (360.0 / eyeNum);
  return (int)angle;
}
