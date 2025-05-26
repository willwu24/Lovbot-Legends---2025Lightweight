#include "defense.h"

// --- Constants ---
double MOVE_OFFSET = 10.0;
const int minDefenseSpeed = 30;
const int maxDefenseSpeed = 55;
const int defenseSpeedDiff = (maxDefenseSpeed - minDefenseSpeed) / 2;

// --- State ---
int whiteDir = -1;
int prevWhiteDetected = -1;
int lastGoalAngle = -1;
int defenseSpeedOffset = 0;

// --- Ball possession state ---
unsigned long firstHasBall = -1;
unsigned long firstBallFront = -1;
bool gettingBall = false;
unsigned long firstGettingBall = -1;

// --- PID Setup ---
double distBallClosest = 20, defBallDist, distSpeedRatio;
double distKp = 1.5, distKi = 0.0, distKd = 0.10;
PID distPID(&defBallDist, &distSpeedRatio, &distBallClosest, distKp, distKi, distKd, DIRECT);

double angleBallClosest = 0, defBallAngle, angleSpeedRatio;
double angleKp = 1.5, angleKi = 0.0, angleKd = 0.1;
PID anglePID(&defBallAngle, &angleSpeedRatio, &angleBallClosest, angleKp, angleKi, angleKd, REVERSE);

// --- Initialization ---
void setupDefense() {
  distPID.SetOutputLimits(0, 100);
  distPID.SetMode(AUTOMATIC);

  anglePID.SetOutputLimits(0, 100);
  anglePID.SetMode(AUTOMATIC);

  Serial.println("BallSetup Complete");
}

// --- Main defense behavior ---
void defenseMain() {
  setAngleThres(20);

  if (whiteDetected()) {
    whiteMove(getDefenseDir());
    prevWhiteDetected = -1;
  } else {
    if (prevWhiteDetected == -1) {
      prevWhiteDetected = millis();
    } else if (millis() - prevWhiteDetected < 1000) {
      setDir(whiteDir);
    } else {
      setDir(180);  // Default fallback direction
    }
  }

  setSpeed(10 + defenseSpeedOffset);
}

// --- White line alignment movement ---
void whiteMove(int dir) {
  double mag = constrain(getMagnitude(), -1.0, 1.0);

  // Choose the white line direction closer to goal
  int rawWhite = getRawWhiteAngle();
  int goalAngle = getHomeAngle();
  mag = abs(mag) * (getAngleDif(rawWhite, goalAngle) > getAngleDif((rawWhite + 180) % 360, goalAngle) ? -1 : 1);

  double offset = (dir != 360) ? constrain(defenseSpeedOffset, 10, 50) * (mag * 0.7) : 0;
  if (dir > 180) offset *= -1;

  setDir(dir + offset);
}

// --- Defense direction logic ---
int getDefenseDir() {
  int moveAngle = (getEyeAngle() > 180) ? 90 : 270;
  whiteDir = getRawWhiteAngle();

  // Use opposite side of white line if it’s closer to the goal
  if (abs(getAngleDif(whiteDir, getHomeAngle())) > abs(getAngleDif((whiteDir + 180) % 360, getHomeAngle()))) {
    whiteDir = (whiteDir + 180) % 360;
  }

  int defenseDir = (whiteDir + moveAngle) % 360;

  // Adjust speed based on proximity to corners
  double cornerRatio = constrain(1 - abs(getAngleDif(whiteDir, 180)) / 45.0, 0.05, 1.0);
  if ((getEyeAngle() > 180 && whiteDir > 190) || (getEyeAngle() < 180 && whiteDir < 170)) {
    cornerRatio = 1.0;
  }

  double angleRatio = constrain(
    min(abs(getAngleDif(180, getEyeAngle())), abs(getAngleDif(0, getEyeAngle()))) / 90.0, 0.0, 1.0);

  defenseSpeedOffset = cornerRatio * angleRatio * 60;

  // Reverse if defense direction misaligned with ball
  if ((whiteDir < 155 && getEyeAngle() > 180) || (whiteDir > 205 && getEyeAngle() < 180)) {
    defenseDir = (defenseDir + 180) % 360;
  }

  return defenseDir;
}

// --- Reset all temporary defense-related states ---
void resetVariables() {
  firstHasBall = -1;
  gettingBall = false;
  firstBallFront = -1;
  firstGettingBall = -1;
}
