#ifndef GOTOBALL_H
#define GOTOBALL_H

#include <Arduino.h>
#include <PID_v1.h>
#include "camera.h"
#include "compoundEye.h"
#include "player.h"
void setUpBallPID();
void goToBallPID();
void resetBallPID();

#endif