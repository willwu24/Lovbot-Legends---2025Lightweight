#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "func.h"
#include "compass.h"
#include "camera.h"
#include "button.h"
#include "led.h"
#include "dribbler.h"
#include "shooter.h"
#include "display.h"
#include "bluetooth.h"
#include "offense.h"
#include "defense.h"
#include "DFUltraTrig.h"
#include "Grayscale.h"
#include "goToBall.h"
#include "coordinate.h"
#include "coordinateMovement.h"
#include "config.h"

void player();

void dribblerControl();

void shooterControl();
bool shootStrategyCondition();
// void goToBall();



void doWhiteLineOpposite();
void doWhiteLineGoals();


void offense1();
void offense1left();
void offense1right();
void offense2Camera();
void offense2Distance();
void offense3();
void offense4();
void offense5();
void offense6();
void offense6right();
void offense6left();

void offense7Distance();
void offense8();
void offense8right();
void offense8left();
void offense9();

bool hasBall();
void defense();
void dribble();
void backPosition();
void backPosition2();
void whiteLine();
bool shootConditioin();
void miracleButton();

#endif
