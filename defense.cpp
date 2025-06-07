#include "defense.h"

double MOVE_OFFSET = 10.0;
int whiteDir = -1;
int prevWhiteDetected = -1;
int lastGoalAngle = -1;
int defenseSpeedOffset;

unsigned long firstHasBall = -1;
unsigned long firstBallFront = -1;
bool gettingBall = false;
unsigned long firstGettingBall = -1;
//27

// TODO:
/*
- going back to white line when goal angle can't be seen but kinda can
- back white line is out
- slowdown near end of penalty line
- going to center of whiteline when not moving
- pid for angle, then pid for eye value
*/

int minDefenseSpeed = 30;//25
int maxDefenseSpeed = 55; 
int defenseSpeedDiff = (maxDefenseSpeed-minDefenseSpeed)/2;
double distBallClosest, defBallDist, distSpeedRatio;
double distKp=1.5, distKi=0.0, distKd=0.10;
double distPIDMin = 0;
double distPIDMax = 100;
PID distPID(&defBallDist, &distSpeedRatio, &distBallClosest, distKp, distKi, distKd, DIRECT);
double angleBallClosest, defBallAngle, angleSpeedRatio;
double angleKp=1.5, angleKi=0.0, angleKd=0.1;
double anglePIDMin = 0;
double anglePIDMax = 100;
PID anglePID(&defBallAngle, &angleSpeedRatio, &angleBallClosest, angleKp, angleKi, angleKd, REVERSE);


void setupDefense(){
  distBallClosest = 20; // Make sure to test this
  angleBallClosest = 0;
  distPID.SetOutputLimits(distPIDMin, distPIDMax);
  distPID.SetMode(AUTOMATIC);
  anglePID.SetOutputLimits(anglePIDMin, anglePIDMax);
  anglePID.SetMode(AUTOMATIC);
  Serial.println("BallSetup Complete");
}

void defenseMain(){
  setAngleThres(25);
  if(whiteDetected()){
    whiteMove(getDefenseDir());
    prevWhiteDetected = -1;
    defenseSpeedOffset = constrain(defenseSpeedOffset,5,40);
    setSpeed(defenseSpeedOffset);
  }
  else{
    if (blueDetected()){
      if(getUltraBack()<25){
        setSpeed(30);
        if(getEyeValue()<12){
          int ballAngle = getEyeAngle();
          if(ballAngle>180){
            ballAngle = constrain(ballAngle, 280, 359);
          }
          else{
            ballAngle = constrain(ballAngle, 0, 80);
          }
          setDir(ballAngle);
        }
        else{
          setDir((getHomeAngle()+180)%360);
        }
      }
      else{
        setDir(getHomeAngle());
        int backSpeed = (getHomeDistance()-20)/3;
        backSpeed = constrain(backSpeed, 20, 40);
        setSpeed(backSpeed);
      }
    }
  }

}


void whiteMove(int dir){
  double magnitude = getMagnitude();
  magnitude = constrain(magnitude, -1.0, 1.0);

  if (abs(getAngleDif(getRawWhiteAngle(),getHomeAngle()))>abs(getAngleDif((getRawWhiteAngle()+180)%360,getHomeAngle()))){
    magnitude = abs(magnitude)*-1;
  }
  else
  {
    magnitude = abs(magnitude);
  }
  double offset = 0;
  if (dir != 360)
  {
    offset = magnitude* 25.0;
  }

  if (dir > 180){
    offset *=-1;
  }

  double finalAngle = dir + offset;

  setDir(finalAngle);
}

int getDefenseDir(){
  int defenseDir = 0;
  double magnitude = getMagnitude();
  magnitude = constrain(magnitude, -1.0, 1.0);

  whiteDir = (getRawWhiteAngle()+getCompass())%360;
  if(abs(getAngleDif(whiteDir,getHomeAngle()))>abs(getAngleDif((whiteDir+180)%360,getHomeAngle()))){
    whiteDir=(whiteDir+180)%360;
  }
  int moveAngle = 0;
  if (getEyeAngle()>180){
    moveAngle = 90;
  }
  else{
    moveAngle = 270;
  }
  defenseDir = (whiteDir+moveAngle)%360;

  double cornerRatio = 1-abs(getAngleDif(whiteDir, 180))/30.0;
  cornerRatio = constrain(cornerRatio, 0.00, 1.0);

  if ((getEyeAngle() > 180 && whiteDir > 180 && getEyeAngle() < 345) || (getEyeAngle() < 180 && whiteDir < 180 && getEyeAngle() > 15)){
    cornerRatio = 1.0;
  }

  double angleRatio = min(abs(getAngleDif(180, getEyeAngle())),abs(getAngleDif(0, getEyeAngle())))/90.0;
  angleRatio = constrain(angleRatio, 0.2,1.0);

  double distRatio = getEyeValue()/300.0;
  distRatio = constrain(distRatio,0.3,1.0);

  defenseSpeedOffset = cornerRatio*angleRatio*distRatio*100;


  if ((whiteDir < 160 && getEyeAngle() > 180) || (whiteDir > 200 && getEyeAngle() < 180)){
    defenseDir = (defenseDir+180)%360;
  }

  return defenseDir;
}



void resetVariables(){
  firstHasBall = -1;
  gettingBall = false;
  firstBallFront = -1;
  firstGettingBall = -1;
}