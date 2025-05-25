#ifndef DEFENSE_H
#define DEFENSE_H

#include "func.h"
#include "camera.h"
#include "distance.h"
#include "backposition.h"
#include "goToCoordinate.h"
#include <PID_v1.h>
#include "grayscale.h"
#include "compoundEye.h"
#include "dribbler.h"

void setupDefense();
void defenseMain();
void whiteMove(int);
int getDefenseDir();
void resetVariables();

#endif

