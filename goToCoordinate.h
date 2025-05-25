#ifndef GOTOCENTER_H
#define GOTOCENTER_H
#include <Arduino.h>
//#include "Utility.h"
#include "func.h"
#include "Distance.h"
void goToCO();
void setGoToCO(int x, int y);
void processCoordinate();
int getXCO();
int getYCO();
void setXCO(int xx);
void setYCO(int yy);
void advancedParking();
void setParked(bool a);
void setGoToClosest();
void setGoToOrder();
void setExponentialSpeed(int di);
#endif 