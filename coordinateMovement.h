#ifndef COORDINATEMOVEMENT_H
#define COORDINATEMOVEMENT_H

#include "player.h"

void setupCoordinateMovement();
void backPositionDefense();
void moveToCoordinate();
bool isParked(int);
int speedCalculator(int);
int distCalculator(posInfo, posInfo);
int dirCalculator(posInfo, posInfo);
moveInfo blockedCalculator(posInfo, posInfo);
bool changeLocation();
void getNextCO();
void getNextCORandom();
void getNextCORandomAvoid();
void setCenter();
void setNeutralSpot();
void cornerPark();
void setTargetCO();

#endif