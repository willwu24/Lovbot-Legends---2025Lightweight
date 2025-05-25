#include "defense.h"

double MOVE_OFFSET = 15.0;
int whiteTar = -1;
int prevWhite = -1;
int lastGoalAngle = -1;

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
  setMotorMode(0);
  retrieveKicker();
  //Set Variables
  int whiteDir = getWhiteAngle();
  setTarget(0);
  setAngleThres(25);
  // setTurningMode(1);
  if (getAbsAngle(whiteDir-getHomeAngle())>getAbsAngle((whiteDir+180)%360-getHomeAngle())){
    whiteTar = (whiteDir+getCompass())%360;
  }
  else{
    whiteTar = (whiteDir+getCompass()+180)%360;
  }
  int ballDir = (getEyeAngle()-(whiteTar)+360)%360;//(getEyeAngle()-(getHomeAngle()-180)+360)%360;
  //Serial.print(ballDir);
  if(whiteDetected()&&homeDetected()){
    prevWhite = whiteDir;
  }
  if(homeDetected()){
    lastGoalAngle = getHomeAngle();
  }
  defBallDist = getEyeValue();
  // Serial.print(whiteTar);
  //Main code logic
  setSpeed(30);
  if(hasBall()){
    //Serial.print("HAS BALL");
    if(firstHasBall==-1){
      firstHasBall = millis();
    }
    if((millis()-firstHasBall>500||getUltraBack()>90)&&getUltraFront()>20){
      kick();
      resetVariables();
    }
    else{
      setDir((getHomeAngle()+180)%360);
      setSpeed(60);
    }
  }
  else if(gettingBall){
    //Serial.print("GETTING BALL");
    if(firstGettingBall==-1){
      firstGettingBall = millis();
    }
    if(millis()-firstGettingBall>700||(getEyeAngle()>60&&getEyeAngle()<300)||(getUltraBack()>90)){//||getEyeValue()<180
      firstGettingBall=false;
      resetVariables();
    }
    // else{
       goToBallPID();
       setSpeed(60);
    // }
    if(whiteDetected()&&millis()-firstGettingBall>200){
      setDir((getWhiteAngle() + 180) % 360);
    }
  }
  else if(!whiteDetected()){//going back to white line
    //Serial.print("RETURN TO WHITE");
    resetVariables();
    if(getUltraBack()<27){
      if(homeDetected()&&abs(getHomeLeftEdgeAngle()-getHomeRightEdgeAngle()>20)){//TUNE
        // if(getEyeValue()<12||getEyeAngle()<20||getEyeAngle()>340){
        //   setDir(0);
        // }
        // else if(getEyeAngle()<180){//ballRight
        //   setDir(45);
        // }
        // else{
        //   setDir(315);
        // }
        if(getEyeValue()<12||(getEyeAngle()>75&&getEyeAngle()<285)){
          setDir(0);
        }
        else{
          setDir(getEyeAngle());
        }
      }
      else if(lastGoalAngle<180){
        setDir(90);
      }
      else{
        setDir(270);
      }
      // else if(prevWhite!=-1){
      //   setDir(prevWhite);
      // }
    }
    else{
      if(homeDetected()){
        setDir(getHomeAngle());
      }
      else if(lastGoalAngle<180){
        setDir(90);
      }
      else{
        setDir(270);
      }
      // else if(prevWhite!=-1){
      //   setDir(prevWhite);
      // }
    }
  }
  else{
    //Serial.print("ON WHITE");
    if(!homeDetected()){
      //Serial.print("in corner?");
      // prevWhite==-1;
      if(lastGoalAngle<180){
        setDir(45);
      }
      else{
        setDir(315);
      }
      if(abs(getAngleDif(180, getWhiteAngle()))>20&&abs(getAngleDif(0, getWhiteAngle()))>20){ 
        //Serial.print("STUCK");
        setDir(0);
      }
      //Serial.print(getWhiteAngle());
    }
    else if(getUltraBack()>37){//TUNE
      setDir(getHomeAngle());
    }
    else if(getEyeValue() < 12){//no ball, go back to center?//TUNE, not that important tho!
      resetVariables();
      if(getYellowRightEdgeAngle()>140){
        whiteMove(270);
      }
      else if(getYellowLeftEdgeAngle()<195){
        whiteMove(90);
      }
      else{
        whiteMove(360);
      }
      // if(getHomeAngle()>170&&getHomeAngle()<190){
      //   if(getUltraBack()>25){
      //     whiteMove(360);
      //   }
      //   else if(whiteTar>180){
      //     whiteMove(90);
      //   }
      //   else{
      //     whiteMove(270);
      //   }
      // }
      // else if(getHomeAngle()<180){
      //   whiteMove(90);
      // }
      // else{
      //   whiteMove(270);
      // }
    }
    else{ //goToBall
      setMotorMode(1);
      defBallAngle = min(abs(getAngleDif(ballDir,0)),abs(getAngleDif(ballDir,180)));
      // Serial.print(defBallAngle);
      // Serial.print(" ");
      if(getUltraBack()<40&&(abs(getAngleDif(0,whiteTar))>40&&abs(getAngleDif(180,whiteTar))>40)){//reaching end of penalty white line, go back//(abs(getAngleDif(whiteTar,90))<20||abs(getAngleDif(whiteTar,270))<20)
        setMotorMode(0);
        //Serial.print("moving back");
        resetVariables();
        //Serial.print(lastGoalAngle);
        if(lastGoalAngle!=-1){
          if(lastGoalAngle<180){
            whiteMove(90);
          }
          else{
            whiteMove(270);
          }
        }
        else{
          setDir(0);
        }
        if(getHomeAngle()<180){
          if(getEyeAngle()>=160&&getEyeAngle()<340){
            setDir(360);
          }
          else{
            whiteMove(90);
          }
        }
        else{
          if(getEyeAngle()<=200&&getEyeAngle()>30){
            setDir(360);
          }
          else{
            whiteMove(270);
          }
        }
        if(getUltraBack()<15){
          setDir(0);
        }
      }
      else if(ballDir<15||ballDir>345||(ballDir<200&&ballDir>160)){//ball in front or ball behind
        if(getEyeValue()<180||(getEyeAngle()>=20&&getEyeAngle()<=340)){
          resetVariables();
        }
        else{
          if(firstBallFront==-1){
            firstBallFront=millis();
          }
          else if(millis()-firstBallFront>3000){
            gettingBall = true;
          }
        }
        whiteMove(360);
      }
      else if(ballDir<180){//ball left
        resetVariables();
        whiteMove(90);
      }
      else{//ball right
        resetVariables();
        whiteMove(270);
      }
      // distPID.Compute();
      anglePID.Compute();
      // Serial.print(angleSpeedRatio);
      double addition =(defenseSpeedDiff*angleSpeedRatio)/100;//(defenseSpeedDiff * distSpeedRatio) / 100+
      double finalSpeed = addition + minDefenseSpeed;
      if(abs(getAngleDif(0,whiteTar))>15){
        //Serial.print("SLOWDOWN");
        finalSpeed = finalSpeed*0.75;
      }
      if(getEyeAngle()>90&&getEyeAngle()<270){
        finalSpeed = finalSpeed*0.65;
      }
      // finalSpeed = finalSpeed-abs(getAngleDif(180,getHomeAngle()))*0.25;
      setSpeed(finalSpeed);
    }
  }
}


void whiteMove(int dir){
  //behind white line: 0, magnitue negative
  //in front of white line: 180, mangitude positive
  double magnitude = abs(getMagnitude());
  int goalAngle = (getHomeAngle()+getCompass())%360;
  if(getRawWhiteAngle()<90||getRawWhiteAngle()>270){
    // Serial.print("Changing");
    magnitude = abs(magnitude)*-1;
  }
  // Serial.print(getRawWhiteAngle());
  // Serial.print(" ");
  // Serial.print(getMagnitude());
  // Serial.print(" ");
  // Serial.print(magnitude);
  // Serial.print(" ");
  setDir((dir+whiteTar)%360);
  if(dir==STOP){
    setDir(STOP);
  }
  else{
    int dir = getDir();
    //Serial.print(dir);
    if(abs(magnitude)<0.10){
      setDir(dir);
    }
    else{
      int offset = magnitude*MOVE_OFFSET;
      if(abs(getAngleDif(0,whiteTar))>15){
        offset=offset*2;
      }
      // Serial.print(offset);
      // Serial.print(" ");
      if(dir>180){
        offset*=-1;
      }
      setDir(dir+offset);
    }
  }
  if(!whiteDetected()){
    setDir(dir);
  }
}

void resetVariables(){
  firstHasBall = -1;
  gettingBall = false;
  firstBallFront = -1;
  firstGettingBall = -1;
}

