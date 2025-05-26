 #include "GoToBall.h"
//can't go over
//SPEED PID
int minGoToBallSpeed = 30;//40
int maxGoToBallSpeed = 50;//16, 36//36-->46
int speedDiff = maxGoToBallSpeed-minGoToBallSpeed;

//DIRECTION PID
int minDir = 0;
int maxDir = 90;
int dirDiff = maxDir - minDir;

//---------------------------------------//

double ballClosest, ballFarthest, ballDist, speedRatio, distanceRatio;
double speedKp=0.15, speedKi=0.001, speedKd=0.001;// 0.74
double dirKp=0.24, dirKi=0.00, dirKd=0.005;//0.01, 0.3, 0.004,                  0.26

double PIDMinimum = 0;
double PIDMaximum = 100;

PID speedPID(&ballDist, &speedRatio, &ballClosest, speedKp, speedKi, speedKd, DIRECT);

 


PID dirPID(&ballDist, &distanceRatio, &ballFarthest, dirKp, dirKi, dirKd, REVERSE);

//


void setUpBallPID(){
    // Adjust initial closest distance according to new range
    if (getBotNum==0){
      ballClosest = 750;
    }
    else{
      ballClosest = 750; // Make sure to test this//250
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

  setAngleThres(50);

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

  finalSpeed = constrain(finalSpeed, minGoToBallSpeed, maxGoToBallSpeed+5);

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
  setSpeed(finalSpeed);

  if (ballDist < 12) { // Adjust condition for reset based on new range
      resetBallPID();
  }
}



void resetBallPID(){
	speedPID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
	speedPID.SetOutputLimits(-1.0, 0.0);  // Forces maximum down to 0.0
	speedPID.SetOutputLimits(PIDMinimum, PIDMaximum);  // Set the limits back to normal

	dirPID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
	dirPID.SetOutputLimits(-1.0, 0.0);  // Forces maximum down to 0.0
	dirPID.SetOutputLimits(PIDMinimum, PIDMaximum);  // Set the limits back to normal
}