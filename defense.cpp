#include "defense.h"

int lastWhiteDir = 180;
int lastDefenseWhiteTime = 0;
int ballTimer = 0;

double finalHeading;

void defenseMain(){
  setAngleThres(12);
  if (!whiteDetected() && millis() - lastDefenseWhiteTime < 1000){
    setSpeed(15);
    setDir(lastWhiteDir);
  }
  else if (!whiteDetected()){
    goToCoordinate(0,0);
  }
  else
  {
    int tempCorner = (getDefenseDir() + getCompass())%360;
    if ((tempCorner > 160 && tempCorner < 190) || (tempCorner < 200 && tempCorner > 170))
    {
      whiteMove((getDefenseDir() + 180)%360);
      lastWhiteDir = getWhiteAngle();
      lastDefenseWhiteTime = millis();
    }
    else
    {
      whiteMove(getDefenseDir());
      lastWhiteDir = getWhiteAngle();
      lastDefenseWhiteTime = millis();
    }
  }

  // bool ballAhead = whiteDetected() && ((getEyeAngle() < 45) || (getEyeAngle() > 315)) &&  (getEyeValue() > 150);

  // if (ballAhead)
  // {
  //   if (ballTimer == 0){
  //     ballTimer = millis();
  //   }         // start
  //   if (millis() - ballTimer >= 2000)                 // 2 s reached
  //   {
  //       goToBallPID();                                // ❶ chase ball                                 // done this tick
  //   }
  // } 
  // else {
  //   ballTimer = 0;                                    // reset timer
  // }
  // whiteMove(getDefenseDir());
}


void whiteMove(int dir)                       // dir = 90 or 270 (robot frame)
{
  /* 1. White-line vector (robot frame) ----------------------- */
  double mag     = getMagnitude();          // 0 … 1
  double wlAngle = getWhiteAngle();         // 0 … 359 (robot frame)

  double wx = 0.3 * mag * sin(toRadian(wlAngle)); // X: East+
  double wy = 0.3 * mag * cos(toRadian(wlAngle)); // Y: North+

  /* 2. Drive vector (robot frame) ---------------------------- */
  double dx = sin(toRadian(dir));           // ±1
  double dy = cos(toRadian(dir));           // 0

  /* 3. Vector addition -------------------------------------- */
  double fx = wx + dx;
  double fy = wy + dy;

  /* 4. Robot-frame heading ---------------------------------- */
  finalHeading = atan2(fx, fy) * 180.0 / PI;   // −180 … +180
  finalHeading = fmod(finalHeading + 360.0, 360.0);   // 0 … 359.999

  /* 5. Convert to field frame with compass ------------------ */
  // finalHeading = fmod(finalHeading + getCompass(), 360.0);

  setDir(finalHeading);                                // field-absolute
  
  double goalRatio = abs(getAngleDif(180, (getHomeAngle() + getCompass())%360)) / 40.0;
  goalRatio = constrain(goalRatio, 0.0, 0.8);
  goalRatio = 1.0 - goalRatio;

  double cornerRatio;
  int tempCorner = (getDefenseDir() + getCompass())%360;
  if (getEyeAngle() > 180){
    cornerRatio = (270 - tempCorner) / 30;
  }
  else{
    cornerRatio = (tempCorner - 90) / 30;
  }
  cornerRatio = constrain(cornerRatio, 0.0, 1.0);
  cornerRatio = 1.0-cornerRatio;

  int defenseGoalAngle = (getHomeAngle() + getCompass())%360;

  if ((defenseGoalAngle > 180 && getEyeAngle() > 180) || (defenseGoalAngle < 180 && getEyeAngle() < 180)){
    goalRatio = 1.0;
  }

  double angleRatio = min(abs(getAngleDif(180, getEyeAngle())),abs(getAngleDif(0, getEyeAngle())))/90.0;//90.0
  angleRatio = constrain(angleRatio, 0.0,1.0);

  // double distRatio = getEyeValue()/200.0;
  // distRatio = constrain(distRatio,0.8,1.0);
  if ((tempCorner > 160 && tempCorner < 190) || (tempCorner < 200 && tempCorner > 170))
  {
    setSpeed(20);
  }
  else{
    setSpeed(angleRatio*cornerRatio*40);
  }
}

bool isCorner(int angle, int CORNER_TH)
{
    static const int kCorner[4] = {45, 135, 225, 315};

    for (int i = 0; i < 4; i++)
        if (abs(getAngleDif(angle, kCorner[i]) ) < CORNER_TH)
            return true;
    return false;
}


int getDefenseDir()
{
    /* --- 1. Normalise angles -------------------------------- */
    int whiteDir = getWhiteAngle();
    if (whiteDir < 0) whiteDir += 360;
    whiteDir %= 360;

    int ballAngle = getEyeAngle();
    if (ballAngle < 0) ballAngle += 360;
    ballAngle %= 360;

    /* --- 2. Choose ±90° offset based on ball side ------------ */
    int moveAngle;
    if (ballAngle > 180) {
        moveAngle = 270;        // ball is left  → use left offset
    } else {
        moveAngle = 90;         // ball is right → use right offset
    }

    /* --- 3. Decide stripe orientation with hysteresis -------- */
    static int stripeForward = 1;          // remember last state
    const double tol = 0.1736;             // 10° dead-band
    double yComp = cos(whiteDir * M_PI / 180.0);

    if (yComp >  tol) stripeForward = 1;   // clearly forward
    if (yComp < -tol) stripeForward = 0;   // clearly backward

    /* --- 4. Preliminary heading (same formula as before) ----- */
    int defenseDir;
    if (stripeForward) {
        defenseDir = (whiteDir + 360 - moveAngle) % 360;
    } else {
        defenseDir = (whiteDir + moveAngle) % 360;
    }

    /* --- 5. Flip 180° to stand on the correct side ----------- */
    defenseDir = (defenseDir + 180) % 360;

    return defenseDir;                     // 0–359°
}

