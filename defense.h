#ifndef DEFENSE_H
#define DEFENSE_H

#include <PID_v2.h>
#include "func.h"
#include "camera.h"
#include "grayscale.h"
#include "compoundEye.h"
#include "dribbler.h"
#include "player.h"

// --- Defense Module Interface ---

// Initialize defense PID controllers and state
void setupDefense();

// Main control loop for defense behavior
void defenseMain();

// Adjust robot direction while following the white line
void whiteMove(int dir);

// Calculate and return the appropriate defense direction
int getDefenseDir();

void blockGoalie();

void blockGoalie1();

int getHomeDir();

void resetVariables();


#endif  // DEFENSE_H
