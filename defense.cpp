#include "defense.h"

int whiteDir = -1;
int prevWhiteDetected = -1;
int lastGoalAngle = -1;
int defenseSpeedOffset;

long firstHasBall = -1;
long firstBallFront = -1;
bool gettingBall = false;
long firstGettingBall = -1;
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
  retrieveKicker();
  setAngleThres(20);
  // setTarget(0);
  //setMotorMode(1);
  setTurningMode(1);
  if(homeDetected()){
    lastGoalAngle = getHomeAngle();
  }
  if(0){//hasBall()){
    Serial.print("HAS BALL");
    setAngleThres(10);
    firstGettingBall=-1;
    firstBallFront = -1;
    gettingBall = false;
    setSpeed(50);
    setDir(0);
    if(firstHasBall==-1){
      firstHasBall = millis();
    }
    else if(millis()-firstHasBall>50){
      //kick();
      firstHasBall = -1;
    }
  }

  else if(gettingBall){//get ball from neutral spot
    Serial.print("GETTING BALL");
    firstBallFront=-1;
    firstHasBall = -1;
    if(firstGettingBall==-1){
      firstGettingBall = millis();
    }
    else if(millis()-firstGettingBall>600){//went for too long, go back now
      gettingBall=false;
      firstGettingBall = -1;
    }
    else if(whiteDetected()&&millis()-firstGettingBall>300){//hit white line, go back
      gettingBall=false;
      firstGettingBall = -1;
    }
    else{
      goToBallPID();
      //setDir(0);
      setSpeed(40);

    }
    // setSpeed(max(getSpeed(),30));
  }

  else if(whiteDetected()){//on white line
    setTarget(0);
    firstGettingBall=-1;
    gettingBall = false;
    firstHasBall = -1;
    Serial.print("ON WHITE LINE");
    whiteMove(getDefenseDir());
    prevWhiteDetected = -1;
    defenseSpeedOffset = constrain(defenseSpeedOffset,5,45);//TUnE
    setSpeed(defenseSpeedOffset);
    if(abs(getHomeLeftEdgeAngle()-getHomeRightEdgeAngle())<5&&homeDetected()&&(abs(getAngleDif(90, getWhiteAngle()))<10||abs(getAngleDif(270, getWhiteAngle()))<10)){//on the side white line
      if(getUltraBack()>88){//actually side white line, go back    //TUNE
        setDir(getHomeAngle());
        setSpeed(30);
      }
      else{//not there, on the end of the goal white line, go forward
        setDir(0);
        setSpeed(15);
      }
    }
    else if(!homeDetected()){//in the corner
      setSpeed(15);
      if(lastGoalAngle<180){//go back to white line
        setDir(45);
        if(getUltraRight()>100){
          setDir(0);
        }
      }
      else{
        setDir(315);
        if(getUltraLeft()>100){
          setDir(0);
        }
      }
    }
    else if((getEyeAngle()<=60||getEyeAngle()>=300)&&getEyeValue()>250){//ball in front
      Serial.print("ball front");
      if(firstBallFront==-1){
        firstBallFront = millis();
      }
      else if(millis()-firstBallFront>1000){
        // setTarget((getEyeAngle()+getCompass())%360);
        // setAngleThres(15);
        gettingBall = true;
      }
    }
    else{//reset Variables
      firstBallFront = -1;
    }
  }

  else{ //returning to white
    setTarget(0);
    firstGettingBall=-1;
    firstBallFront = -1;
    gettingBall = false;
    firstHasBall = -1;
    if(homeDetected()&&abs(getHomeLeftEdgeAngle()-getHomeRightEdgeAngle())>15||(getHomeDistance()>40&&abs(getHomeLeftEdgeAngle()-getHomeRightEdgeAngle())>3)){//in the middle
      if(getUltraBack()<88){
        setSpeed(15);
        if(getEyeValue()>12){//ball detected, go to ball
          int ballAngle = getEyeAngle();
          if(ballAngle>180){
            ballAngle = constrain(ballAngle, 340, 359);
          }
          else{
            ballAngle = constrain(ballAngle, 0, 20);
          }
          setDir(ballAngle);
        }
        else{//nope, go opposite of goal
          setDir((getHomeAngle()+180)%360);
        }
      }
      else{
        if(homeDetected()){
          setDir(getHomeAngle());
          int backSpeed = (getHomeDistance()-20)/2;
          backSpeed = constrain(backSpeed, 20, 25);
          setSpeed(15);
        }
        else{
          goToCoordinate(0,-220);
          setSpeed(constrain(getSpeed(),20,30));
        }
        // setDir(getHomeAngle());
        // setSpeed(backSpeed);
      }
    }
    else{
      setSpeed(20);
      if(1){//(getUltraBack()<100){
        if(lastGoalAngle>180){
          setDir(270);
        }
        else{
          setDir(90);
        }
      }
      // else{
      //   goToCoordinate(0,-220);
      //   setSpeed(constrain(getSpeed(),20,30));
      // }
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
  double maxOffset = 20.0;
  if(abs(getAngleDif(whiteDir,180))>30){
    maxOffset = 35.0;
  }
  if (dir != 360)
  {
    offset = magnitude * maxOffset;//TUNE 25.0
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
  int ballAngle = -1;

  whiteDir = (getRawWhiteAngle()+getCompass())%360;
  if(abs(getAngleDif(whiteDir,getHomeAngle()))>abs(getAngleDif((whiteDir+180)%360,getHomeAngle()))){
    whiteDir=(whiteDir+180)%360;
  }
  if(0){//(abs(getAngleDif(180, whiteDir))<40){
    ballAngle = int((getEyeAngle()+getCompass()-(whiteDir-180)*1.5))%360;
  }
  else{
    ballAngle = (getEyeAngle()+getCompass())%360;
  }

  int moveAngle = 0;
  if (ballAngle>180){
    moveAngle = 90;
  }
  else{
    moveAngle = 270;
  }
  defenseDir = (whiteDir+moveAngle)%360;

  //Speed controls

  double cornerRatio = 1-abs(getAngleDif(whiteDir, 180))/20.0;//30.0
  cornerRatio = constrain(cornerRatio, 0.00, 1.0);

  if ((getEyeAngle() > 180 && whiteDir > 180 && getEyeAngle() < 345) || (getEyeAngle() < 180 && whiteDir < 180 && getEyeAngle() > 15)){
    cornerRatio = 1.0;
  }

  if ((getEyeAngle() > 180 && whiteDir > 200) || (getEyeAngle() < 180 && whiteDir < 160)){
    cornerRatio = 0.4;
  }

  double angleRatio = min(abs(getAngleDif(180, getEyeAngle())),abs(getAngleDif(0, getEyeAngle())))/80.0;//90.0
  angleRatio = constrain(angleRatio, 0.0,1.0);

  double distRatio = getEyeValue()/200.0;
  distRatio = constrain(distRatio,0.8,1.0);

  defenseSpeedOffset = cornerRatio*angleRatio*120;

  if (whiteDir < 160 && (getEyeAngle() > 180||getEyeAngle()<=20)){
    defenseDir = 90;
    defenseSpeedOffset = 15;
  }
  else if(whiteDir > 200 && (getEyeAngle() < 180||getEyeAngle()>=340)){
    defenseDir = 270;
    defenseSpeedOffset = 15;
  }
  // if ((whiteDir < 160 && (getEyeAngle() > 180||getEyeAngle()<=20)) || (whiteDir > 200 && (getEyeAngle() < 180||getEyeAngle()>=340))){ //reached end of white line
  //   defenseDir = (defenseDir+180)%360; //moving back 
  //   defenseSpeedOffset = 10;
  // }

  return defenseDir;
}



void resetVariables(){
  firstHasBall = -1;
  gettingBall = false;
  firstBallFront = -1;
  firstGettingBall = -1;
}