#include "offense.h"

int reached = 0;
int lastReached = 0;

int leftright = 0;
int checkDir = 0;
int lastWhite = 0;
int lastAntiDefenseWhite = 0;
int wasWhite = 0;
int currWhiteSpeed = 0;

int randomAround = 0;
//int transmitData = 0;
int lastWhiteAngle;
int lastAntiDefenseWhiteAngle;
int targetSet = 0;
int firstWhiteFlag = 0;
int targetSetTime = 0;
int minWhiteSpeed = 60;
int maxWhiteSpeed = 80;
int lastTarget = 0;

int fieldPosition = 0;

int decideOffense = 0;

bool leftWhiteReached = false;
bool rightWhiteReached = false;
bool backWhiteReached = false;
bool whiteReached = false;
bool runningAntiDefense = false;

int lastOffenseSet = 0;
int targetFlag = 0;
int checkPointTime = 0;

int cornerArrived = 0;

int lastFrontReached = 0;

int hasKick = 0;
int lastLaserTime = 0;
int firstBall = 0;

unsigned long lastHasBall = -1;

//corner going out may be due to the goal speedup stuff

void offenseMain(){
  // Serial.println(getTarget());
  retrieveKicker();
  // Serial.println(leftright);
  int hasBallSpeedOffset;
  setTarget(0);
  setLEDState(1,0);

  setTurningMode(0);
  // setTurnSpeed(20);

  // if (getOpposingDistance() > 80 && !hasBall() && homeDetected() && getEyeValue() > 15 && whiteDetected() && getHomeAngle() < 215 && getHomeAngle() > 145 && getHomeDistance() < 70 && millis()-getLastRoleSwitch() > 2000){
  //   setRobotRole(DEFENSE);
  // }

  // if (opposingDetected() && (!whiteDetected())){
  //   int offenseTarget;
  //   offenseTarget = (getOpposingAngle()+getCompass())%360;
  //   if (offenseTarget < 40 || offenseTarget > 320)
  //   {
  //     targetSet = millis();
  //     setTarget(offenseTarget);
  //   }
  // }
  // else if (!whiteDetected() && millis() - targetSet > 500)
  // {
  //   setTarget(0);
  // }
  setTarget(0);



  if(whiteDetected() && firstBall != 1){
    if(wasWhite==0){
      wasWhite = 1;
      currWhiteSpeed = getSpeed()*1.30;
    }
    ///currWhiteSpeed = constrain(currWhiteSpeed, 50, 90);
    //setSpeed(currWhiteSpeed);
    setSpeed(40);
    //Serial.println("Running White Line");
    // setAngleThres(90);
    // setTurningMode(1);
    // setTurnSpeed(7);
    // whiteReached = true;

    // setLEDState(0, 1);
    setMotorMode(0);
    resetBallPID();
    targetFlag = 0;
    int dirAngle = (getWhiteAngle() + 180) % 360;
    if (dirAngle > 80 && dirAngle < 100)
    {
      leftWhiteReached = true;
    }
    else if (dirAngle > 260 && dirAngle < 280)
    {
      rightWhiteReached = true;
    }

    lastWhite = millis();
    lastWhiteAngle = (getWhiteAngle() + 180) % 360;
    if(0){//((getHomeDistance()<60&&homeDetected()&&!getFlip())||(getUltraFront()>40&&getUltraBack()<35&&getUltraRight()<45&&getUltraRight()>30&&getEyeAngle()>180)||(getUltraFront()>40&&getUltraBack()<35&&getUltraLeft()<45&&getUltraLeft()>30&&getEyeAngle()<180)){
      //Serial.print("tangent");
      setLEDState(1,1);
      if(getEyeAngle()<180){
        setDir((80+getWhiteAngle() + 180)%360);
      }
      else{
        setDir((280+getWhiteAngle() + 180)%360);
      }
    }
    else if(0){//(getInCorner()&&!getFlip()){
      cornerPark();
    }
    else{
      setLEDState(1,0);
      setDir((getWhiteAngle() + 180) % 360);
    }

    double magnitude = getMagnitude();
    //setSpeed(minWhiteSpeed+(maxWhiteSpeed-minWhiteSpeed)*(1-magnitude)*0.5);
    // if (!getFlip()) 
    // {
    //   setSpeed(25);
    // } else 
    // {
    //   setSpeed(30);
    // }
  }
  else if (((millis() - lastWhite < 150 && !getFlip()))|| (millis() - lastWhite < 2000 && getFlip())) //|| (millis() - lastWhite < 1000 && getFlip())) && firstBall != 1) 
  {
    //Serial.println("Running White Line 50ms");
    // setTurningMode(1);
    // reached = 0;
    // setTarget(0);
    //setSpeed(currWhiteSpeed);
    
    setMotorMode(0);
    resetBallPID();
    // setLEDState(0, 0);
    if(0){//((getHomeDistance()<60&&homeDetected()&&!getFlip())||(getUltraFront()>40&&getUltraBack()<35&&getUltraRight()<45&&getUltraRight()>40&&getEyeAngle()>180)||(getUltraFront()>40&&getUltraBack()<35&&getUltraLeft()<45&&getUltraLeft()>40&&getEyeAngle()<180)){
      setLEDState(1,1);
      if(getEyeAngle()<180){
        setDir((80+lastWhiteAngle + 180)%360);
      }
      else{
        setDir((280+lastWhiteAngle + 180)%360);
      }
    }
    else if(0){//(getInCorner()&&!getFlip()){
      cornerPark();
    }
    else{
      setLEDState(1,0);
      setDir((lastWhiteAngle) % 360);
    }

    // if (!getFlip()) 
    // {
    //   setSpeed(15);
    // } else 
    // {
    //   setSpeed(20);
    // }
    // setSpeed(25);
    //setSpeed(minWhiteSpeed);
  }
  else
  {
    wasWhite = 0;
    setMotorMode(0);
    int m = hasBall();
    // Serial.print(analogRead(LAZER_PIN));
    // Serial.print(" ");
    //Serial.print(m);
    if (hasBall()){ //&& getEyeValue() > 150 && (getEyeAngle() < 100 || getEyeAngle() > 260)
      //Serial.print("Has ball");
      setTurningMode(0);
      // if (opposingDetected() && (!whiteDetected())){
      //   int offenseTarget;
      //   offenseTarget = (getOpposingAngle()+getCompass())%360;
      //   targetSet = millis();
      //   if (offenseTarget < 180){
      //     offenseTarget = constrain(offenseTarget, 0, 60);
      //   }
      //   else
      //   {
      //     offenseTarget = constrain(offenseTarget, 300, 360);
      //   }
      //   setTarget(offenseTarget);
      // }
      // else if (!whiteDetected())
      // {
      //   setTarget(0);
      // }

      // setSpeed(38);
      // setDir(0);
      goToBallPID();
      if(getCompass()<10||getCompass()>350){
        if(getUltraLeft()>60&&getUltraRight()>60){
          lastTarget=0;
        }
        else if(getUltraLeft()<getUltraRight()){
          lastTarget=60;
        }
        else{
          lastTarget=300;
        }
      }
      // if(lastHasBall ==-1){
      //   lastHasBall = millis();
      // }
      // if(millis()-lastHasBall>100){
      //   Serial.print("KICK!");
      //   kick();
      // }


      // if(getUltraFront()<25){
      //   // setSpeed(70);
      //   grabBall();
      // }
      // grabBall();
      // setTarget(lastTarget);
      // setDir(0);
      if(homeDetected()){
        setDir((getHomeAngle()+180)%360);
      }
      else{
        setDir(lastTarget);
      }
      //Serial.print("I have the ball");
      //transmit(1);
      // if(getUltraLeft()<60){
      //   setTarget(45);
      // }
      // if ((getOpposingAngle() < 10 || getOpposingAngle() > 350) && getOpposingDistance() < 85){
      //   setDribbler(66,ON);
      //   if (millis()-lastReached > 50){
      //     kick();
      //   }
      // }
      kick();
      setSpeed(50);
    }
    else if (receive()==1){
        //Serial.print("Other robot has ball");
        setDir(STOP);
        setCenter();
        //moveToCoordinate();
    }
    else if (getEyeValue() < 12){
      lastHasBall = -1;
      setMotorMode(0);
      runningAntiDefense = false;
      whiteReached = false;
      reached = 0;
      hasKick = 0;
      firstBall = 0;
      setTurningMode(0);
      // setTarget(0);
      resetBallPID();
      //Serial.print("Backposition");
      setCenter();
      //moveToCoordinate();
      setDir(STOP);

      // if (opposingDetected()){
      //   // int offenseTarget = (getOpposingAngle()+getCompass())%360;
      //   if (getCompass() < 6 || getCompass() > 354){
      //     if (getOpposingDistance() > 80 && getOpposingDistance() < 86){
      //       setDir(STOP);
      //     }
      //     else
      //     {
      //       if (getOpposingDistance() > 83){
      //         setDir(0);
      //         setSpeed(18);
      //       }
      //       else
      //       {
      //         setDir(180);
      //         setSpeed(18);
      //       }
      //     }
      //   }
      //   else{
      //     setTarget(0);
      //     if (getCompass() > 180){
      //       setDir(270);
      //       int backPositionSpeed = (359-getCompass())/1.6;
      //       backPositionSpeed = constrain(backPositionSpeed, 12, 30);
      //       setSpeed(backPositionSpeed);
      //     }
      //     else
      //     {
      //       setDir(90);
      //       int backPositionSpeed = getCompass()/1.6;
      //       backPositionSpeed = constrain(backPositionSpeed, 12, 30);
      //       setSpeed(backPositionSpeed);
      //     }
      //   }
      // }
      // else
      // {
      //   // setTarget(0);
      //   setDir(STOP);
      // }
    }
    else{
      lastHasBall = -1;
      //Serial.print("GO TO BALL");
      setMotorMode(1);
      runningAntiDefense = false;
      whiteReached = false;
      reached = 0;
      hasKick = 0;
      firstBall = 0;
      // setTarget(0);
      setTurningMode(0);

      goToBallPID();
      //Serial.print(getAbsAngle(getEyeAngle()-(lastWhiteAngle+180)%360));
      if(millis() - lastWhite < 500&&((getAbsAngle(getEyeAngle()-(lastWhiteAngle+180)%360)<50&&!getFlip())||(getAbsAngle(getEyeAngle()-(lastWhiteAngle+180)%360)<60&&getAbsAngle(getEyeAngle()-180)<60))){
        //Serial.print("SLOWDOWN");
        setSpeed(20);
      }
      setSpeed(30);
      // if(getInCorner()){
      //   cornerPark();
      // }
      // if(getUltraBack()<20&&getUltraRight()<20)
    } 
  }
  // Serial.print(getDir());
  // Serial.print(" ");
  // Serial.println(firstBall);
  // setDribbler(30,OFF);
  setTurningMode(0);
}


  //..........................................................................
  //.............ddddd........................................................
  //............ddddddd.......................................................
  //............ddddddd.......................................................
  //............ddddddd.......................................................
  //............ddddddd.......................................................
  //............ddddddd.......................................................
  //.....dddddddddddddd....oooooooooo......nnnnnnnnnnnnnn.......eeeeeeeee.....
  //....ddddddddddddddd...ooooooooooooo....nnnnnnnnnnnnnnn.....eeeeeeeeeee....
  //...dddddddddddddddd..ooooooooooooooo...nnnnnnnnnnnnnnnn..eeeeeeeeeeeeee...
  //..ddddddddddddddddd..ooooooooooooooo...nnnnnnnnnnnnnnnn..eeeeeeeeeeeeeee..
  //..ddddddddddddddddd.oooooooo.oooooooo..nnnnnnnnnnnnnnnn.eeeeeeeeeeeeeeee..
  //.ddddddd...dddddddd.ooooooo...ooooooo..nnnnnnn...nnnnnn.eeeeeee...eeeeee..
  //.ddddddd....ddddddddoooooo.....oooooo..nnnnnn....nnnnnn.eeeeeeeeeeeeeeee..
  //.dddddd.....ddddddddoooooo.....ooooooo.nnnnnn....nnnnnn.eeeeeeeeeeeeeeee..
  //.dddddd.....ddddddddoooooo.....ooooooo.nnnnnn....nnnnnn.eeeeeeeeeeeeeeee..
  //.dddddd.....ddddddddoooooo.....ooooooo.nnnnnn....nnnnnn.eeeeeeeeeeeeeeee..
  //.dddddd.....ddddddddoooooo.....oooooo..nnnnnn....nnnnnn.eeeeeeeeeeeeeeee..
  //.ddddddd....ddddddddoooooo.....oooooo..nnnnnn....nnnnnn.eeeeee.....eeeee..
  //.dddddddd..dddddddd.ooooooo...ooooooo..nnnnnn....nnnnnn.eeeeeee...eeeeee..
  //..ddddddddddddddddd.ooooooooooooooooo..nnnnnn....nnnnnn..eeeeeeeeeeeeeee..
  //..ddddddddddddddddd..ooooooooooooooo...nnnnnn....nnnnnn..eeeeeeeeeeeeeee..
  //...dddddddddddddddd...ooooooooooooo....nnnnnn....nnnnnn...eeeeeeeeeeeee...
  //....ddddddddddddddd....ooooooooooo.....nnnnnn....nnnnnn....eeeeeeeeeee....
  //......dddddd.............ooooooo.............................eeeeeee......
  //..........................................................................
void grabBall(){
  while(hasBall() && getCompass()<45&&!whiteDetected()){
    if(getUltraLeft()>getUltraRight()){ //motor values might be wrong... negatives :( 
      Serial.print("turning left");
      setMotors(-20, -20, -30, -30);
    }
    else{
      Serial.print("turning right");
      setMotors(30, 30, 20, 20);
    }
  }
}