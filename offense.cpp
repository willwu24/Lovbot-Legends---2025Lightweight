#include "offense.h"

// === State Tracking ===
int lastWhite = 0;
int wasWhite = 0;
int currWhiteSpeed = 0;

int lastWhiteAngle = 0;
int lastTarget = 0;
int firstBall = 0;

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
  }

  // === Recently Detected White Line ===
  else if ((millis() - lastWhite < 100 && !getFlip()) ||
           (millis() - lastWhite < 2000 && getFlip())) {
    Serial.println("Running White Line 50ms");

    resetBallPID();
    setDir(lastWhiteAngle);
  }

  // === Default Behavior ===
  else {
    if (hasBall()){
      goToBallPID();
    }
    else if (getEyeValue() < 12){
      setDir(STOP);
    }
    else
    {
      goToBallPID();
    }
  }
}

// === Turning Strategy ===
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
