#ifndef COORDINATEMOVEMENT_H
#define COORDINATEMOVEMENT_H

#include "player.h"

// Initialization
void setupCoordinateMovement();

// Movement functions
void backPositionDefense();
void moveToCoordinate();
void cornerPark();

// Coordinate utilities
bool isParked(int dist);
int speedCalculator(int dist);
int distCalculator(posInfo curr, posInfo target);
int dirCalculator(posInfo curr, posInfo target);
moveInfo blockedCalculator(posInfo curr, posInfo target);

// Location control
bool changeLocation();
void getNextCO();
void getNextCORandom();
void getNextCORandomAvoid();
void setNeutralSpot();
void setCenter();
void setTargetCO();

#endif
