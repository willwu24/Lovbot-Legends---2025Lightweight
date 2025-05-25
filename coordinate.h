#ifndef COORDINATE_H
#define COORDINATE_H

#include "player.h"

posInfo findCOByCorner(int, int, int, int);
posInfo findCOByCenter(int, int, int, int);
posInfo findCOBySmallest(int, int, int, int);
void getCoordinate();
posInfo findCO(int, int, int, int);
bool isXBlocked(int, int);
bool isYBlocked(int, int);
int getX();
int getY();
int getXBlocked();
int getYBlocked();
void setOdoShiftX(int);
posInfo getCurrPosition();

#endif