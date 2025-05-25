#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "func.h"

void setupButton();
void updateButton();//update the state of the buttons

bool getButtonState(int which);//return the state of given button
bool buttonPressed(int which);//return true if a button is being pressed

void clearButtonState();//clear all the state of the buttons(set them to 0)
bool buttonLongPressed(int which);//return true if given button is pressed and holded

void checkButton();
#endif
