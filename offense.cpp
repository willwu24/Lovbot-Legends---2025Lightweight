#include "offense.h"

// === State Tracking ===
int lastWhite = 0;
int wasWhite = 0;
int currWhiteSpeed = 0;

int lastWhiteAngle = 0;
int lastTarget = 0;
int firstBall = 0;

int frontStart = 100, frontHard = 75;
int backStart  = 100, backHard  = 75;
int leftStart  = 100, leftHard  = 60;
int rightStart = 100, rightHard = 60;

bool turnSet = false;

unsigned long lastHasBall = -1;

// === Main Offense Control ===
void offenseMain() {
  retrieveKicker();
  setTurningMode(1);
  setAngleThres(40);

  // === White Line Detected ===
  // if (whiteDetected() && firstBall != 1) {
  if (0){
    turnSet = false;
    setTarget(0);
    setMotorMode(0);
    resetBallPID();

    int dirAngle = (getWhiteAngle() + 180) % 360;
    lastWhite = millis();
    lastWhiteAngle = dirAngle;

    setDir(dirAngle);
    if (!getFlip()){
      setSpeed(30);
    }
    else
    {
      setSpeed(35);
    }
  }

  // === Recently Detected White Line ===
  else if ((millis() - lastWhite < 100 && !getFlip()) ||
           (millis() - lastWhite < 1000 && getFlip())) {
    Serial.println("Running White Line 50ms");

    resetBallPID();
    setDir(lastWhiteAngle);
  }

  // === Default Behavior ===
  else {
    if(hasBall()){
      goToBallPID();
      applyAirWall();
      //kick();
      setSpeed(50);
    }
    else if (getEyeValue() < 12){
      turnSet = false;
      setTarget(0);
      goToCoordinate(0, 0);
    }
    else
    {
      setTarget(0);
      if (!turnSet && getEyeValue() > 230 && getUltraFront() < 200 && (abs(getUltraLeft() - getUltraRight())) > 30){
        Serial.print("Turning Condition: ");
        Serial.print(turnSet);
        Serial.print("    ");
        if (getUltraRight() < getUltraLeft() && getUltraRight() < 110 )
        {
          setTarget(330);
          turnSet = true;
        }
        else if (getUltraLeft() < getUltraRight() && getUltraLeft() < 110){
          setTarget(30);
          turnSet = true;
        }
        Serial.println(getTarget());
      }
      else if (!turnSet){
        setTarget(0);
      }

      if (getEyeValue() < 180){
        turnSet = false;
      }
    
      goToBallPID();
      applyAirWall();
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

double calculateRepelEffect(int distance, int startThreshold, int hardThreshold, double ratio) {
    return ratio * constrain((startThreshold - distance) / (double)(startThreshold - hardThreshold), 0.0, 1.0);
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

  if (getDir() > 90 && getDir() <270){
    repelY += calculateRepelEffect(ultraB, backStart, backHard);
  }
  else
  {
    repelY -= calculateRepelEffect(ultraF, frontStart, frontHard, 0.6);
  }

  if (getDir() > 180){
    repelX += calculateRepelEffect(ultraL, leftStart, leftHard);
  }
  else
  {
    repelX -= calculateRepelEffect(ultraR, rightStart, rightHard);
  }

  double repelMag = sqrt(repelX * repelX + repelY * repelY);

  // Serial.print("RepelX: "); Serial.print(repelX);
  // Serial.print("  RepelY: "); Serial.print(repelY);
  // Serial.print("  Magnitude: "); Serial.print(repelMag);

  int currDir = getDir();    // 0–359
  int currSpeed = getSpeed(); // Magnitude

  double angleRad = (90 - currDir) * M_PI / 180.0;
  double x = cos(angleRad) * currSpeed;
  double y = sin(angleRad) * currSpeed;

  // Serial.print("X: "); Serial.print(x);
  // Serial.print("  Y: "); Serial.print(y);

  const double MAX_REPEL_FORCE = 50.0; // You can tune this value//60
  x += repelX * MAX_REPEL_FORCE;
  y += repelY * MAX_REPEL_FORCE;

  // Step 4: Convert back to polar coordinates
  double combinedMag = sqrt(x * x + y * y);
  int newSpeed = (int)combinedMag;
  int newDir = 360; // default STOP

  if (combinedMag > 0.01) {
    double combinedAngle = atan2(y, x);
    newDir = ((int)(90 - combinedAngle * 180.0 / M_PI + 360)) % 360;
  }

  // Step 5: Apply or print
  setSpeed(newSpeed);
  setDir(newDir);

  // Serial.print("  NewDir: "); Serial.print(newDir);
  // Serial.print("  NewSpeed: "); Serial.println(newSpeed);
}