#ifndef GOTOBALL_H
#define GOTOBALL_H

#include <Arduino.h>
#include <PID_v1.h>
#include "camera.h"
#include "compoundEye.h"
#include "player.h"

// ---------------------- Function Declarations ---------------------- //

// Initializes ball-tracking PID controllers
void setUpBallPID();

// Controls robot to approach the ball using PID
void goToBallPID();

// Resets the PID state to prevent overshooting or instability
void resetBallPID();

#endif // GOTOBALL_H
