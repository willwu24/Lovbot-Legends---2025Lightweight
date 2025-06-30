 #include "GoToBall.h"
//can't go over
//SPEED PID
int minGoToBallSpeed = 20;//30
int maxGoToBallSpeed = 55;//16, 36//36-->46   50
int speedDiff = maxGoToBallSpeed-minGoToBallSpeed;

//DIRECTION PID
int minDir = 0;
int maxDir = 90;
int dirDiff = maxDir - minDir;

//---------------------------------------//

double ballClosest, ballFarthest, ballDist, speedRatio, distanceRatio;
double speedKp=1.1, speedKi=0.001, speedKd=0.001;// 0.74
double dirKp=0.42, dirKi=0.05, dirKd=0.0;//0.01, 0.3, 0.004,                  0.24

double PIDMinimum = 0;
double PIDMaximum = 100;

PID speedPID(&ballDist, &speedRatio, &ballClosest, speedKp, speedKi, speedKd, DIRECT);

 


PID dirPID(&ballDist, &distanceRatio, &ballFarthest, dirKp, dirKi, dirKd, REVERSE);

//


void setUpBallPID(){
    // Adjust initial closest distance according to new range
    if (getBotNum==0){
      ballClosest = 250;
    }
    else{
      ballClosest = 250; // Make sure to test this//250
    }
    ballFarthest = 12;

    speedPID.SetOutputLimits(PIDMinimum, PIDMaximum);
    speedPID.SetMode(AUTOMATIC);
    dirPID.SetOutputLimits(PIDMinimum, PIDMaximum);
    dirPID.SetMode(AUTOMATIC);

    Serial.println("BallSetup Complete");
}

void goToBallPID(){
  //Serial.print("GOING TO BALL ");
  
	int ballDir = getEyeAngle();
	ballDist = getEyeValueSmooth();

  bool ballBehind = 90 < ballDir && ballDir < 270;
  bool ballRight = ballDir < 180;
  bool goalRight = getHomeAngle()<180&&homeDetected();
  int absAngle = ballDir;

  if (!ballRight) {
    absAngle = 360 - ballDir;
 	}
  
  dirPID.Compute();
  speedPID.Compute();

  // double addition = speedDiff * (speedRatio / 100.0);
  double finalSpeed = maxGoToBallSpeed * (speedRatio/100.0);

  finalSpeed = constrain(finalSpeed, minGoToBallSpeed, maxGoToBallSpeed);

  double ballAngleRatio;
  if (ballDir > 180){
    ballAngleRatio = (360.0 - ballDir)/180.0;
  }
  else{
    ballAngleRatio = ballDir/180.0;
  }

  ballAngleRatio = constrain(ballAngleRatio,0.0,1.0);


  double offset = (dirDiff) * (distanceRatio/100.0) * (ballAngleRatio);

	if (!ballRight) {
	    offset *= -1;
	}

  double finalDirection = ballDir + offset;
  setDir(finalDirection);
  setSpeed(60);

  if (ballDist < 12) { // Adjust condition for reset based on new range
      resetBallPID();
  }

    Serial.print("speed: ");
    Serial.print(getSpeed());
    Serial.print("angle: ");
    Serial.print(getEyeAngle());
    Serial.print("value: ");
    Serial.print(getEyeValue());
    Serial.print("back: ");
    Serial.println(speedRatio);
}



void resetBallPID(){
	speedPID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
	speedPID.SetOutputLimits(-1.0, 0.0);  // Forces maximum down to 0.0
	speedPID.SetOutputLimits(PIDMinimum, PIDMaximum);  // Set the limits back to normal

	dirPID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
	dirPID.SetOutputLimits(-1.0, 0.0);  // Forces maximum down to 0.0
	dirPID.SetOutputLimits(PIDMinimum, PIDMaximum);  // Set the limits back to normal
}