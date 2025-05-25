 #include "GoToBall.h"
//can't go over
//SPEED PID
int minGoToBallSpeed = 35;//40
int maxGoToBallSpeed = 45;//16, 36//36-->46
int speedDiff = maxGoToBallSpeed-minGoToBallSpeed;

//DIRECTION PID
int minDir = 0;
int maxDir = 90;
int dirDiff = maxDir - minDir;

//---------------------------------------//

double ballClosest, ballFarthest, ballDist, speedRatio, dirRatio;
double speedKp=0.78, speedKi=0.001, speedKd=0.001;// 0.74
double dirKp=0.18, dirKi=0.00, dirKd=0.005;//0.01, 0.3, 0.004,                  0.26

double PIDMinimum = 0;
double PIDMaximum = 100;

PID speedPID(&ballDist, &speedRatio, &ballClosest, speedKp, speedKi, speedKd, DIRECT);

 


PID dirPID(&ballDist, &dirRatio, &ballFarthest, dirKp, dirKi, dirKd, REVERSE);

//


void setUpBallPID(){
    // Adjust initial closest distance according to new range
    if (getBotNum==0){
      ballClosest = 750;
    }
    else{
      ballClosest = 250; // Make sure to test this//250
    }
    ballFarthest = 15;

    speedPID.SetOutputLimits(PIDMinimum, PIDMaximum);
    speedPID.SetMode(AUTOMATIC);
    dirPID.SetOutputLimits(PIDMinimum, PIDMaximum);
    dirPID.SetMode(AUTOMATIC);

    Serial.println("BallSetup Complete");
}

void goToBallPID(){
  //Serial.print("GOING TO BALL ");
  
	int ballDir = getEyeAngle();
	ballDist = getEyeValue();

  setAngleThres(90);

  bool ballBehind = 90 < ballDir && ballDir < 270;
  bool ballRight = ballDir < 180;
  bool goalRight = getHomeAngle()<180&&homeDetected();
  int absAngle = ballDir;

  if (!ballRight) {
    absAngle = 360 - ballDir;
 	}
  
  dirPID.Compute();


  speedPID.Compute();
  double addition = speedDiff * (speedRatio / 100.0);
  double finalSpeed = addition + minGoToBallSpeed;


  double speedSlowdown;
  double angleSlowdown;
  double offenseAngleRatio;
  double offenseDistanceRatio;

  int offenseTarget = (getOpposingAngle()+getCompass())%360;
  if (offenseTarget > 180){
    offenseAngleRatio = (360-offenseTarget)/360.0;
  }
  else
  {
    offenseAngleRatio = offenseTarget/360.0;
  }
  offenseDistanceRatio = (100-getOpposingDistance())/100.0;
  offenseDistanceRatio = constrain(offenseDistanceRatio, 0.0, 1.0);
  offenseDistanceRatio = 1.0/offenseDistanceRatio;

  speedSlowdown = offenseAngleRatio*offenseDistanceRatio*0.8;
  angleSlowdown = offenseAngleRatio*offenseDistanceRatio*0.6;
  speedSlowdown = constrain(speedSlowdown, 0.0, 1.0);
  angleSlowdown = constrain(angleSlowdown, 0.0, 1.0);
  



  // if (yellowDetected() && blueDetected()){
  //   if (getYellowDistance() < getBlueDistance()){
  //   }
  //   else
  //   {
  //     //Use Blue
  //   }
  // }
  // else if (yellowDetected()){

  // }
  // else if (blueDetected()){

  // // }

  // if (getCompass() > 60 && getCompass() < 300 && getEyeValue() > 200){
  //   finalSpeed = finalSpeed * 0.25;
  // }
  // else if (opposingDetected() && ((offenseTarget > 180 && getEyeAngle() < 180) || (offenseTarget < 180 && getEyeAngle() > 180))){
  //   finalSpeed = finalSpeed * (1-speedSlowdown);
  // }
  if(homeDetected()){
    // Serial.print("Goal Detected");
    if(ballRight==goalRight){
      finalSpeed = finalSpeed+10;
      //Serial.print("Speedup");
    }
    else{
      finalSpeed = finalSpeed-10;
      //Serial.print("Slowdown");
    }
  }
  finalSpeed = constrain(finalSpeed, minGoToBallSpeed, maxGoToBallSpeed+5);
  // Serial.print(yellowAngleRatio);
  // Serial.print(" ");
  // Serial.print(getYellowAngle());
  // Serial.print(" ");
  // Serial.print(yellowDistanceRatio);
  // Serial.print(" ");
  // Serial.print(speedSlowdown);
  // Serial.print(" ");
  // Serial.print((yellowTarget > 180 && getEyeAngle() < 180) || (yellowTarget < 180 && getEyeAngle() > 180));
  // Serial.print(" ");

  // Serial.println(finalSpeed);

  double angleConstant = (-4.28*pow(10,-5)*absAngle)+(0.0105*absAngle)+(-0.05);//-4.68*pow 0.0142, 0.135s
  angleConstant = constrain(angleConstant, 0, 1);
  double offset = (dirDiff) * (dirRatio/100) * (angleConstant);
  // if (getEyeAngle() > 270 || getEyeAngle () < 90){
  //   offset = offset * (1.0-angleSlowdown);
  //   // offset
  // }


	if (!ballRight) {
	    offset *= -1;
	}

  double finalDirection = ballDir + offset;
  // if(getUltraRight()<30&&!ballRight){
  //   finalDirection = 270;
  // }
  // if(getUltraLeft()<30&&ballRight){
  //   finalDirection = 90;
  // }
  // if(getUltraFront()>40&&getUltraBack()<35&&getUltraLeft()>80&&getUltraRight()<40){
  //   finalDirection = 0;
  // }
  // if(getUltraFront()>40&&getUltraBack()<35&&getUltraRight()>80&&getUltraLeft()<40){
  //   finalDirection = 0;
  // }
  setDir(finalDirection);

  // finalSpeed = constrain(finalSpeed, 13, 40);
  setSpeed(finalSpeed);
  // Serial.print(speedRatio);
  // Serial.print(" ");


  if (ballDist < 12) { // Adjust condition for reset based on new range
      resetBallPID();
  }

  // Serial.print("Ball Distance: ");
  // Serial.print(ballDist);
  // Serial.print(" ");

  // Serial.print("Ball Angle: ");
  // Serial.print(ballDir);
  // Serial.print(" ");

  // Serial.print("Dir: ");
  // Serial.print(finalDirection);
  // Serial.print(" ");

  // Serial.print("Speed: ");
  // Serial.print(finalSpeed);
  // Serial.println(" ");
}

void resetBallPID(){
	speedPID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
	speedPID.SetOutputLimits(-1.0, 0.0);  // Forces maximum down to 0.0
	speedPID.SetOutputLimits(PIDMinimum, PIDMaximum);  // Set the limits back to normal

	dirPID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
	dirPID.SetOutputLimits(-1.0, 0.0);  // Forces maximum down to 0.0
	dirPID.SetOutputLimits(PIDMinimum, PIDMaximum);  // Set the limits back to normal
}