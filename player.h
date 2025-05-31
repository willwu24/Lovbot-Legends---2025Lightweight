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

// === Main Control ===
void player();
bool hasBall();

// === Dribbler and Shooter ===
void dribblerControl();
void shooterControl();
bool shootStrategyCondition();

#endif
