#include "offense.h"

// === State Tracking ===
int lastWhite = 0;
int wasWhite = 0;
int currWhiteSpeed = 0;

int lastWhiteAngle = 0;
int lastTarget = 0;
int firstBall = 0;

int lastHasBall = 0;
int targetSet = 0;
int lastTargetSet = 0;

int lastAround = 0;

const int frontStart = 200, frontStop = 0, frontReverse = 0;
const int backStart = 180, backStop = 70, backReverse = 0;
const int leftStart = 180, leftStop = 50, leftReverse = 0;
const int rightStart = 180, rightStop = 50, rightReverse = 0;

bool turnSet = false;

// === Main Offense Control ===
void offenseMain() {
  retrieveKicker();
  setTurningMode(1);
  setAngleThres(45);

  // === White Line Detected ===
  if (whiteDetected() && firstBall != 1) {
    changeSendArr(0, 0);
  // if (0){
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
      lastTargetSet = millis();
      if (millis() - lastAround > 150){
        kick();
      }
      goToBallPID();
      applyAirWall();
    }
    else if (getEyeValueSmooth() < 12){
      changeSendArr(0, 1);
      resetBallPID();
      targetSet = 0;
      setTarget(0);
      goToCoordinate(0, 0);
    }
    else
    {
      targetSet = 0;
      lastAround = millis();
      setTarget(0);
      goToBallPID();
      applyAirWall();
      if(inCorner()){
        setDir(STOP);
        changeSendArr(0, 1);
      }
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

double calculateRepelEffect(int distance,
                            int startThreshold,
                            int stopThreshold,
                            int reverseThreshold)
{
    // Assumes: startThreshold > stopThreshold > reverseThreshold
    if (distance >= startThreshold) {
        return 1.0;                                       // full ahead
    }
    else if (distance >= stopThreshold) {
        // +1 → 0   across [start … stop]
        return (double)(distance - stopThreshold) /
                       (startThreshold - stopThreshold);
    }
    else if (distance >= reverseThreshold) {
        //  0 → –1  across [stop … reverse]
        return (double)(distance - stopThreshold) /
                       (stopThreshold - reverseThreshold);
    }
    else {
        return -1.0;                                      // full reverse
    }
}


void applyAirWall()
{
    /* 1. Read sensors & current command */
    int ultraL = getUltraLeftSmooth();
    int ultraR = getUltraRightSmooth();
    int ultraF = getUltraFrontSmooth();
    int ultraB = getUltraBackSmooth();

    int dir   = getDir();     // 0–359  (0 = North, 90 = East)
    int speed = getSpeed();   // magnitude, same units you use elsewhere

    /* 2. Convert (dir,speed) → Cartesian (x,y)  */
    double angleRad = (90.0 - dir) * M_PI / 180.0;   // CW to math CCW
    double x = cos(angleRad) * speed;   // +x = East/right
    double y = sin(angleRad) * speed;   // +y = North/forward

    /* 3. Pick a sensor for each axis and get its ratio */
    double repelXRatio, repelYRatio;

    if (x >= 0) {   // moving / pointing RIGHT → use right sensor
        repelXRatio = calculateRepelEffect(ultraR,
                                           rightStart, rightStop, rightReverse);
    } else {        // moving LEFT → use left sensor
        repelXRatio = calculateRepelEffect(ultraL,
                                           leftStart,  leftStop,  leftReverse);
    }

    if (y >= 0) {   // moving FORWARD → use front sensor
        repelYRatio = calculateRepelEffect(ultraF,
                                           frontStart, frontStop, frontReverse);
    } else {        // moving BACKWARD → use back sensor
        repelYRatio = calculateRepelEffect(ultraB,
                                           backStart,  backStop,  backReverse);
    }

    /* 4. Apply the ratios (scale / stop / reverse) */
    x *= repelXRatio;
    y *= repelYRatio;

    /* 5. Convert back to polar coordinates */
    double newMag = sqrt(x * x + y * y);
    int    newSpeed = (int)newMag;
    int    newDir   = 360;            // 360 → “stop / undefined”

    if (newMag > 0.01) {
        double newAng = atan2(y, x);  // –π … +π  (CCW from +x)
        newDir = ((int)(90.0 - newAng * 180.0 / M_PI + 360.0)) % 360;
    }

    /* 6. Send to the drivetrain */
    setSpeed(newSpeed);
    setDir(newDir);
}

bool inCorner(){
  bool ballFront = getEyeAngle()<30||getEyeAngle()>330;
  if(ballFront&&getUltraFront()<80&&(getUltraLeft()<90||getUltraRight()<90)&&getHomeDistance()>100){
    return true;
  }
  return false;
}