#include "compoundEye.h"

const int eyeNum = 18;

// Thresholds for mapping analog port values to eye sectors (0–17)
int eyePortThres[eyeNum] = { 32, 64, 95, 127, 158, 190, 222, 253, 285, 317, 347, 380, 410, 442, 474, 499, 531, 562};
//                           0   20  40  60   80   100  120  140  160  180  200  220  240  260  280  300  320  340
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

  // int frontAngle = analogRead(A3);  // Front IR sensor
  // if (frontAngle > 60 && frontAngle < 180) {
  //   if (frontAngle < 100) {
  //     degree = 10;
  //   } else if (frontAngle < 130) {
  //     degree = 0;
  //   } else {
  //     degree = 350;
  //   }
  // }

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

#define EYE_WIN 5  

static int  bufEye[EYE_WIN];
static int  idxEye  = 0;
static int  cntEye  = 0;

/* -----------------  public smoothed accessor ------------------- */
int getEyeValueSmooth()
{
    int raw = getEyeValue();                 // original eye sensor reading
    bufEye[idxEye] = raw;                    // push into ring buffer
    idxEye = (idxEye + 1) % EYE_WIN;
    if (cntEye < EYE_WIN) cntEye++;         // ramp-up during first calls
    return trimmedMean5(bufEye, cntEye);     // spike-free, smoothed value
}