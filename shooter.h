#ifndef SHOOTER_H
#define SHOOTER_H

#include "func.h"

// === Kicker State Definitions ===
#define ON  1
#define OFF 0

// === Kicker Control Functions ===
void setupKicker();
void kick();
void retrieveKicker();
void printKickerStatus();

#endif
