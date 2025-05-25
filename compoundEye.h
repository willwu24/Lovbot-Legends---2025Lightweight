#ifndef COMPOUNDEYE_H
#define COMPOUNDEYE_H

#include <Arduino.h>
#include "func.h"

#define BALL_THRES 40

//for analog compound eye
#define EYE_PORT_PIN A10

//for analog compound eye
#define EYE_VALUE_PIN A11

void eyeSetup(); 
int getEyePort();
int getEyeValue();
//Get angle of where ball is
int getEyeAngle();
int getEyeAngleSmooth();
//tells if there is ball



#endif
