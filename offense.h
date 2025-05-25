#ifndef OFFENSE_H
#define OFFENSE_H

#include "func.h"
<<<<<<< HEAD
#include "distance.h"
=======
>>>>>>> 69cc2c2 (deleted unused files)
#include "compass.h"
#include "camera.h"
#include "button.h"
#include "led.h"
#include "dribbler.h"
#include "shooter.h"
#include "display.h"
#include "bluetooth.h"
#include "offense.h"
#include "grayscale.h"
#include "goToBall.h"
#include "defense.h"
#include "player.h"

void offenseMain();

void grabBall();
void lightweightAimShoot();
void lightweightCornerShoot();
void lightweightPullAround();
void whitePullAround();

void cornerShoot();
void goToCorner();
// void aimShoot();
void hideShoot();
void hideShootRight();
void hideShootLeft();

void frontShoot();

// void hideShootDistance();
// void hideShootDistanceLeft();
// void hideShootDistanceRight();

// void hideShootFront();
// void hideShootFrontLeft();
// void hideShootFrontRight();


// void hideSpin();
// void hideSpinRight();
// void hideSpinLeft();
void backSpin();
void backSpinWhite();
// void backSpinSide();

// void superteamStrategy();

// void frontShoot();

#endif
