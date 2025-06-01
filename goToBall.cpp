#include "GoToBall.h"

// ---------------------- SPEED PID PARAMETERS ---------------------- //
int minGoToBallSpeed = 20;        // Minimum robot speed
int maxGoToBallSpeed = 40;        // Maximum robot speed
int speedDiff = maxGoToBallSpeed - minGoToBallSpeed;

// ---------------------- DIRECTION PID PARAMETERS ---------------------- //
int minDir = 0;
int maxDir = 90;
int dirDiff = maxDir - minDir;

// ---------------------- PID Variables ---------------------- //
double ballClosest, ballFarthest, ballDist;
double speedRatio, distanceRatio;

double speedKp = 0.15, speedKi = 0.001, speedKd = 0.001;
double dirKp = 0.30, dirKi = 0.00, dirKd = 0.005;

double PIDMinimum = 0;
double PIDMaximum = 100;

PID speedPID(&ballDist, &speedRatio, &ballClosest, speedKp, speedKi, speedKd, DIRECT);
PID dirPID(&ballDist, &distanceRatio, &ballFarthest, dirKp, dirKi, dirKd, REVERSE);

// ---------------------- Setup Function ---------------------- //
void setUpBallPID() {
    if (getBotNum == 0) {
        ballClosest = 750;
    } else {
        ballClosest = 750;
    }
    ballFarthest = 15;

    speedPID.SetOutputLimits(PIDMinimum, PIDMaximum);
    speedPID.SetMode(AUTOMATIC);

    dirPID.SetOutputLimits(PIDMinimum, PIDMaximum);
    dirPID.SetMode(AUTOMATIC);

    Serial.println("BallSetup Complete");
}

// ---------------------- Go To Ball Function ---------------------- //
void goToBallPID() {
    int ballDir = getEyeAngle();
    ballDist = getEyeValue();

    setAngleThres(50);

    bool ballBehind = (ballDir > 90 && ballDir < 270);
    bool ballRight = (ballDir < 180);
    bool goalRight = (getHomeAngle() < 180 && homeDetected());

    int absAngle = ballRight ? ballDir : 360 - ballDir;

    dirPID.Compute();
    speedPID.Compute();

    double addition = speedDiff * (speedRatio / 100.0);
    double finalSpeed = constrain(addition + minGoToBallSpeed, minGoToBallSpeed, maxGoToBallSpeed + 5);

    double ballAngleRatio = (ballDir > 180) ? (360.0 - ballDir) / 180.0 : ballDir / 180.0;
    ballAngleRatio = constrain(ballAngleRatio, 0.0, 1.0);

    double offset = dirDiff * (distanceRatio / 100.0) * ballAngleRatio;
    if (!ballRight) {
        offset *= -1;
    }

    double finalDirection = ballDir + offset;

    setDir(finalDirection);
    setSpeed(finalSpeed);

    if (ballDist < 12) {
        resetBallPID();
    }
}

// ---------------------- Reset PID Function ---------------------- //
void resetBallPID() {
    // Temporarily restrict outputs to force reset
    speedPID.SetOutputLimits(0.0, 1.0);
    speedPID.SetOutputLimits(-1.0, 0.0);
    speedPID.SetOutputLimits(PIDMinimum, PIDMaximum);

    dirPID.SetOutputLimits(0.0, 1.0);
    dirPID.SetOutputLimits(-1.0, 0.0);
    dirPID.SetOutputLimits(PIDMinimum, PIDMaximum);
}
