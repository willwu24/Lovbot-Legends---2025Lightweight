#include "offense.h"

// === State Tracking ===
int lastWhite = 0;
int wasWhite = 0;
int currWhiteSpeed = 0;

int lastWhiteAngle = 0;
int lastTarget = 0;
int firstBall = 0;

const int minWhiteSpeed = 60;
const int maxWhiteSpeed = 80;

unsigned long lastHasBall = -1;

// === Main Offense Control ===
void offenseMain() {
  retrieveKicker();
  setTarget(0);
  setTurningMode(1);
  setAngleThres(30);

  // === White Line Detected ===
  if (whiteDetected() && firstBall != 1) {
    setMotorMode(0);
    resetBallPID();

    int dirAngle = (getWhiteAngle() + 180) % 360;
    lastWhite = millis();
    lastWhiteAngle = dirAngle;

    setDir(dirAngle);
    setSpeed(50);
  }

  // === Recently Detected White Line ===
  else if ((millis() - lastWhite < 100 && !getFlip()) ||
           (millis() - lastWhite < 2000 && getFlip())) {
    Serial.println("Running White Line 50ms");

    resetBallPID();
    setDir(lastWhiteAngle);
    setSpeed(50);
  }

  // === Default Behavior ===
  else {
    wasWhite = 0;

    if (hasBall()) {
      goToBallPID();

      // Determine last target based on compass and ultrasonic
      if (getCompass() < 10 || getCompass() > 350) {
        if (getUltraLeft() > 60 && getUltraRight() > 60) {
          lastTarget = 0;
        }
        else if (getUltraLeft() < getUltraRight()) {
          lastTarget = 60;
        }
        else {
          lastTarget = 300;
        }
      }

      if (lastHasBall == -1) {
        lastHasBall = millis();
      }

      if (millis() - lastHasBall > 100) {
        Serial.print("KICK!");
        kick();
      }

      if (getUltraFront() < 25) {
        grabBall();
      }

      if (homeDetected()) {
        setDir((getHomeAngle() + 180) % 360);
      } else {
        setDir(lastTarget);
      }

      kick();
      setSpeed(50);
    }

    // === Received Pause Signal ===
    else if (receive() == 1) {
      setDir(STOP);
    }

    // === Lost Ball ===
    else if (getEyeValue() < 12) {
      lastHasBall = -1;
      firstBall = 0;
      resetBallPID();
      setCenter();
      setDir(STOP);
    }

    // === Searching for Ball ===
    else {
      lastHasBall = -1;
      firstBall = 0;
      goToBallPID();
      setSpeed(30);
    }
  }
}

// === Grab Ball Rotation Logic ===
void grabBall() {
  while (hasBall() && getCompass() < 45 && !whiteDetected()) {
    if (getUltraLeft() > getUltraRight()) {
      Serial.print("turning left");
      setMotors(-20, -20, -30, -30);
    }
    else {
      Serial.print("turning right");
      setMotors(30, 30, 20, 20);
    }
  }
}
