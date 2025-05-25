#include "offense.h"

int lastWhite = 0;
int wasWhite = 0;
int currWhiteSpeed = 0;

int lastWhiteAngle;
int minWhiteSpeed = 60;//maybe useful
int maxWhiteSpeed = 80;//unused but maybe useful
int lastTarget = 0;

int firstBall = 0;//not sure

unsigned long lastHasBall = -1;

//corner going out may be due to the goal speedup stuff

void offenseMain(){
  // Serial.println(getTarget());
  retrieveKicker();
  setTarget(0);

  setTurningMode(0);

  int tempOdoX;
  //odo reset code, not used right now
  
  // if(getXBlocked()==0){
  //   if(getUltraRight()<getUltraLeft()){
  //     tempOdoX = (80-min(getUltraRight(),80));
  //   }
  //   else{
  //     tempOdoX = (80-min(getUltraLeft(),80))*-1;
  //   }
  //   setOdoShiftX(tempOdoX);
  //   Serial.print("tempOdo: ");
  //   Serial.print(tempOdoX);
  // }
  // setTurnSpeed(20);


  setTarget(0);



  if(whiteDetected() && firstBall != 1){
    Serial.println("Running White Line");
    setMotorMode(0);
    resetBallPID();
    int dirAngle = (getWhiteAngle() + 180) % 360;

    lastWhite = millis();
    lastWhiteAngle = (getWhiteAngle() + 180) % 360;
    //following white line in home goal code

    // if(0){//((getHomeDistance()<60&&homeDetected()&&!getFlip())||(getUltraFront()>40&&getUltraBack()<35&&getUltraRight()<45&&getUltraRight()>30&&getEyeAngle()>180)||(getUltraFront()>40&&getUltraBack()<35&&getUltraLeft()<45&&getUltraLeft()>30&&getEyeAngle()<180)){
    //   //Serial.print("tangent");
    //   setLEDState(1,1);
    //   if(getEyeAngle()<180){
    //     setDir((80+getWhiteAngle() + 180)%360);
    //   }
    //   else{
    //     setDir((280+getWhiteAngle() + 180)%360);
    //   }
    // }
    // else if(0){//(getInCorner()&&!getFlip()){
    //   cornerPark();
    // }
    // else{
    setDir((getWhiteAngle() + 180) % 360);
    // }

    //different speed controls

    //setSpeed(minWhiteSpeed+(maxWhiteSpeed-minWhiteSpeed)*(1-magnitude)*0.5);
    // if (!getFlip()) 
    // {
    //   setSpeed(25);
    // } else 
    // {
    //   setSpeed(30);
    // }
    // setCenter();
    // moveToCoordinate();
    // setSpeed(40);
        // if(wasWhite==0){
    //   wasWhite = 1;
    //   currWhiteSpeed = getSpeed()*1.30;
    // }
    ///currWhiteSpeed = constrain(currWhiteSpeed, 50, 90);
    //setSpeed(currWhiteSpeed);
    setSpeed(50);
  }
  else if (((millis() - lastWhite < 100 && !getFlip()))|| (millis() - lastWhite < 1000 && getFlip())) //|| (millis() - lastWhite < 1000 && getFlip())) && firstBall != 1) 
  {
    Serial.println("Running White Line 50ms");
    
    setMotorMode(0);
    resetBallPID();

    //code for following white line in home goal

    // if(0){//((getHomeDistance()<60&&homeDetected()&&!getFlip())||(getUltraFront()>40&&getUltraBack()<35&&getUltraRight()<45&&getUltraRight()>40&&getEyeAngle()>180)||(getUltraFront()>40&&getUltraBack()<35&&getUltraLeft()<45&&getUltraLeft()>40&&getEyeAngle()<180)){
    //   setLEDState(1,1);
    //   if(getEyeAngle()<180){
    //     setDir((80+lastWhiteAngle + 180)%360);
    //   }
    //   else{
    //     setDir((280+lastWhiteAngle + 180)%360);
    //   }
    // }
    // else if(0){//(getInCorner()&&!getFlip()){
    //   cornerPark();
    // }
    // else{
      setLEDState(1,0);
      setDir((lastWhiteAngle) % 360);
    //}

    //different speed controls

    // if (!getFlip()) 
    // {
    //   setSpeed(15);
    // } else 
    // {
    //   setSpeed(20);
    // }
    // setSpeed(25);
    //setSpeed(minWhiteSpeed);
    setSpeed(50);
    //setSpeed(currWhiteSpeed);
  }
  else
  {
    //wasWhite = 0; used for white line speed control
    setMotorMode(0);
    if (hasBall()){
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
      if(millis()-lastHasBall>100){
        Serial.print("KICK!");
        kick();
      }


      if(getUltraFront()<25){
        // setSpeed(70);
        grabBall();
      }
      // else{
      //   kick();
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
      //kick();
      if(getY()>35){
        setSpeed(30);
      }
      else{
        setSpeed(50);
      }
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
      firstBall = 0;
      setTurningMode(0);
      // setTarget(0);
      resetBallPID();
      //Serial.print("Backposition");
      setCenter();
      //moveToCoordinate();
      setDir(STOP);
    }
    else{
      lastHasBall = -1;
      //Serial.print("GO TO BALL");
      setMotorMode(0);
      firstBall = 0;
      // setTarget(0);
      setTurningMode(0);

      goToBallPID();
      setSpeed(40);
      //Serial.print(getAbsAngle(getEyeAngle()-(lastWhiteAngle+180)%360));
      // if(millis() - lastWhite < 500&&((getAbsAngle(getEyeAngle()-(lastWhiteAngle+180)%360)<50&&!getFlip())||(getAbsAngle(getEyeAngle()-(lastWhiteAngle+180)%360)<60&&getAbsAngle(getEyeAngle()-180)<60))){
      //   //Serial.print("SLOWDOWN");
      //   setSpeed(20);
      // }
      // if((getEyeAngle() > 10 && getEyeAngle() < 170 && getX() > 0) || (getEyeAngle() < 350 && getEyeAngle() > 190 && getX() < 0)){
      //   setSpeed(40-abs(getX())*1);
      // }
      // else
      // {
      //   setSpeed(40);
      // }
      // if(abs(getX())>35||abs(getY())>35){
      //   setSpeed(10);
      // }
      // if(getInCorner()){
      //   cornerPark();
      // }
      // if(getUltraBack()<20&&getUltraRight()<20)
      updateWall();
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

void updateWall(){
  double dir = getDir();
  Serial.print(" Dir:");
  Serial.print(dir);
  double speed = getSpeed();
  double vector = (360.0-dir+90.0)*3.14159265/180.0; 
  double x = max(0.0,speed)*cos(vector);
  double y = max(0.0,speed)*sin(vector);
  double adjX=x;
  double adjY=y;
  double scaleX = (40.0-abs(getX()))/20.0;
  double scaleY = (40.0-abs(getY()))/20.0;
  // if(abs(getX())<30){
  //   adjX=x*min(1.0,scale);
  // }
  if((getX()>0&& getEyeAngle() > 0 && getEyeAngle() < 180) || (getX()<0 && getEyeAngle() < 360 && getEyeAngle() > 180)){
    adjX=x*scaleX;
    Serial.print("adjust X");
  }
  if((getY()<0 && getEyeAngle() < 270 && getEyeAngle() > 90) || (getY()>0 && (getEyeAngle() > 270 || getEyeAngle() < 90))){
    Serial.print("adjust Y");
    adjY=y*scaleY;
  }
  
  double adjustedSpeed = sqrt(adjX*adjX+adjY*adjY);
  double adjustedAngle = (atan2(adjY,adjX));
  adjustedAngle = fmod((450-adjustedAngle * 180.0/3.14159265),360.0);
  if(adjustedAngle<0){ 
    adjustedAngle+=360.0;
  }
  if(adjustedAngle>=360){
    adjustedAngle-= 360.0;
  }
  Serial.print(" xVector:");
  Serial.print(x);
  Serial.print(" yVector:");
  Serial.print(y);
  Serial.print(" scaleX:");
  Serial.print(scaleX);
  Serial.print(" scaleY:");
  Serial.print(scaleY);
  Serial.print(" adjustedSpeed:");
  Serial.print(adjustedSpeed);
  Serial.print(" adjustedAngle");
  Serial.println(adjustedAngle);
  if(abs(getX())>20||abs(getY())>20)
  {
      setDir(adjustedAngle);
      setSpeed(adjustedSpeed*0.75);
  }

  // wallDistX = 55-getX();
}


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