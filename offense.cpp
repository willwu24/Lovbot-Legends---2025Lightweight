#include "offense.h"

// === State Tracking ===
int lastWhite = 0;
int wasWhite = 0;
int currWhiteSpeed = 0;

int lastWhiteAngle = 0;
int lastTarget = 0;
int firstBall = 0;

int frontStart = 100, frontHard = 50;
int backStart  = 100, backHard  = 50;
int leftStart  = 100, leftHard  = 50;
int rightStart = 100, rightHard = 50;

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
  else if ((millis() - lastWhite < 50 && !getFlip()) ||
           (millis() - lastWhite < 1000 && getFlip())) {
    Serial.println("Running White Line 50ms");

    resetBallPID();
    setDir(lastWhiteAngle);
  }

  // === Default Behavior ===
  else {
    if (0){//(hasBall()){
      goToBallPID();
      //kick();
    }
    else if (getEyeValue() < 12){
      goToCoordinate(0, 0);
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

double calculateRepelEffect(int distance, int startThreshold, int hardThreshold) {
    return constrain((startThreshold - distance) / (double)(startThreshold - hardThreshold), 0.0, 1.0);
}

void applyAirWall(){
  int ultraL = getUltraLeft();
  int ultraR = getUltraRight();
  int ultraF = getUltraFront();
  int ultraB = getUltraBack();

  int dir = getDir();
  int speed = getSpeed();

  double repelX = 0.0;
  double repelY = 0.0;

  
  repelY -= calculateRepelEffect(ultraF, frontStart, frontHard);
  repelY += calculateRepelEffect(ultraB, backStart, backHard);
  repelX += calculateRepelEffect(ultraL, leftStart, leftHard);
  repelX -= calculateRepelEffect(ultraR, rightStart, rightHard);

  double repelMag = sqrt(repelX * repelX + repelY * repelY);

  Serial.print("RepelX: "); Serial.print(repelX);
  Serial.print("  RepelY: "); Serial.print(repelY);
  Serial.print("  Magnitude: "); Serial.print(repelMag);

  int currDir = getDir();    // 0–359
  int currSpeed = getSpeed(); // Magnitude

  double angleRad = (90 - currDir) * M_PI / 180.0;
  double x = cos(angleRad) * currSpeed;
  double y = sin(angleRad) * currSpeed;

  Serial.print("X: "); Serial.print(x);
  Serial.print("  Y: "); Serial.println(y);
}
