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
    int tempCorner = (getWhiteAngle() + getCompass())%360;
    if (((tempCorner > 30 && tempCorner < 60) || (tempCorner > 210 && tempCorner < 240)) && getEyeAngle() < 180)
    {
      whiteMove((270 + getWhiteAngle() + getCompass())%360);
    }
    else if (((tempCorner > 120 && tempCorner < 150) || (tempCorner > 300 && tempCorner < 330)) && getEyeAngle() < 180){
      whiteMove((90 + getWhiteAngle() + getCompass())%360);
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
  finalHeading = fmod(finalHeading + getCompass(), 360.0);

  setDir(finalHeading);                                // field-absolute

  double goalRatio = abs(getAngleDif(180, getHomeAngle())) / 30.0;
  goalRatio = constrain(goalRatio, 0.2, 1.0);

  double angleRatio = min(abs(getAngleDif(180, getEyeAngle())),abs(getAngleDif(0, getEyeAngle())))/90.0;//90.0
  angleRatio = constrain(angleRatio, 0.0,1.0);

  // double distRatio = getEyeValue()/200.0;
  // distRatio = constrain(distRatio,0.8,1.0);

  setSpeed(angleRatio*goalRatio*40);
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
  int whiteDir = getWhiteAngle();
  int ballAngle = getEyeAngle();
  /* 1.  Decide which side the ball is on --------------------- */
  int moveAngle;                   // +90° = right (east), +270° = left (west)
  if (ballAngle > 180) {           // ball on the left (west) side
      moveAngle = 270;
  } else {                         // ball on the right (east) side
      moveAngle = 90;
  }

  /* 2.  Add or subtract the offset, depending on whiteDir ---- */
  int defenseDir;
  if (whiteDir > 90 && whiteDir < 270) {
      // stripe normal points forward → add the offset
      defenseDir = (whiteDir + 360 - moveAngle) % 360;  // (whiteDir - moveAngle) wrapped
  } else {
      // stripe normal points backward → subtract the offset
      defenseDir = (whiteDir + moveAngle) % 360;
  }
  return defenseDir;               // 0‒359°
}
