#include "defense.h"

int lastWhiteDir = 180;
int lastDefenseWhiteTime = 0;
int ballTimer = 0;

double finalHeading;

void defenseMain(){
  // transmit(2);
  setAngleThres(20);
  if (!whiteDetected() && millis() - lastDefenseWhiteTime < 1500){
    setSpeed(40);
    setDir(lastWhiteDir);
  }
  else if (!whiteDetected()){
    if (lastWhiteDir < 90 || lastWhiteDir > 270){
      setDir(0);
      setTarget(0);
      setSpeed(40);
    }
    else if (homeDetected()){
      setDir(getHomeAngle());
      setTarget(0);
      setSpeed(40);
      // goToCoordinate(0,-100);
    }
    else
    {
      goToCoordinate(0,-100);
    }
  }
  else
  {
    int tempCorner = (getDefenseDir() + getCompass())%360;
    if (getEyeValue() < 12){
      whiteMove(getHomeDir());
      setSpeed(abs(getAngleDif(180,getHomeAngle())) * 1.0);
    }
    else if ((tempCorner > 160 && tempCorner < 190) || (tempCorner < 200 && tempCorner > 170))
    {
      whiteMove((getDefenseDir() + 180)%360);
    }
    else
    {
      whiteMove(getDefenseDir());
    }
    lastWhiteDir = getWhiteAngle();
    lastDefenseWhiteTime = millis();
  }
}


void whiteMove(int dir)                       // dir = 90 or 270 (robot frame)
{
  /* 1. White-line vector (robot frame) ----------------------- */
  double mag     = getMagnitude();          // 0 … 1
  double wlAngle = getWhiteAngle();         // 0 … 359 (robot frame)
  int tempCorner = (getDefenseDir() + getCompass())%360;

  double defenseTuning = min(abs(getAngleDif(180,tempCorner)), abs(getAngleDif(0,tempCorner)))/90.0;

  defenseTuning = 1.0 - defenseTuning;
  defenseTuning = constrain(defenseTuning, 0.1, 1.0);
  defenseTuning = map(defenseTuning, 0.10,1.0,0.10,1.5);

  double wx = defenseTuning * mag * sin(toRadian(wlAngle)); // X: East+
  double wy = defenseTuning * mag * cos(toRadian(wlAngle)); // Y: North+

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
  // double cornerRatio = min(abs(getAngleDif(180,tempCorner)), abs(getAngleDif(0,tempCorner)))/90.0;
  // cornerRatio = 1.0 - cornerRatio;
  // cornerRatio = constrain(cornerRatio, 0.2, 1.0);
  // Serial.println(cornerRatio);
  // defenseTuning = map(defenseTuning, 0.2,1.0,0.2,1.0);

  if (getEyeAngle() > 180){
    cornerRatio = (270 - tempCorner) / 30.0;
  }
  else{
    cornerRatio = (tempCorner - 90) / 30.0;
  }
  cornerRatio = constrain(cornerRatio, 0.0, 1.0);
  cornerRatio = 1.0-cornerRatio;


  int defenseGoalAngle = (getHomeAngle() + getCompass())%360;

  if ((defenseGoalAngle > 180 && getEyeAngle() > 180) || (defenseGoalAngle < 180 && getEyeAngle() < 180)){
    goalRatio = 1.0;
  }

  double angleRatio = min(abs(getAngleDif(180, getEyeAngle())),abs(getAngleDif(0, getEyeAngle())))/90.0;//90.0
  // angleRatio = constrain(angleRatio, 0.0,0.6);
  // angleRatio = 1.0 - (exp(2.0 * (1.0 - angleRatio)) - 1.0) / (exp(2.0) - 1.0);   // high slope near 0, flat near 1
  // angleRatio = 1.0 - angleRatio;
  angleRatio = constrain(angleRatio, 0.0,1.0);


  // double distRatio = getEyeValue()/200.0;
  // distRatio = constrain(distRatio,0.8,1.0);
  if ((tempCorner > 160 && tempCorner < 190) || (tempCorner < 200 && tempCorner > 170))
  {
    setSpeed(30);
  }
  else
  {
    double finalDefenseSpeed = angleRatio*cornerRatio*100.0;
    finalDefenseSpeed = constrain(finalDefenseSpeed,15,80);
    setSpeed(finalDefenseSpeed);
  }

  Serial.print(angleRatio);
  Serial.print(" ");
  Serial.println(getSpeed());
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

    // Step 2: Choose ±90° offset based on which side of the goal-to-ball line you're on
    int ballBehind = (ballAngle + 180) % 360;
    int diff = getAngleDif(getHomeAngle(), ballBehind);  // signed angle from goal to point behind ball

    int moveAngle;
    if (0){//(homeDetected()){
      if (diff > 0) {
          moveAngle = 90;   // goal is left of ballBehind → move right
      } else {
          moveAngle = 270;  // goal is right of ballBehind → move left
      }
    }
    else
    {
      if (ballAngle < 180) {
          moveAngle = 90;   // goal is left of ballBehind → move right
      } else {
          moveAngle = 270;  // goal is right of ballBehind → move left
      }
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

int getHomeDir(){
    /* --- 1. Normalise angles -------------------------------- */
    int whiteDir = getWhiteAngle();
    if (whiteDir < 0) whiteDir += 360;
    whiteDir %= 360;


    /* --- 2. Choose ±90° offset based on ball side ------------ */
    int moveAngle;
    if (getHomeAngle() > 180) {
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