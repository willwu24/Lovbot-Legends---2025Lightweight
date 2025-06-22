#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "func.h"

// Initializes button pins as inputs
void setupButton();

// Updates the state of all buttons
void updateButton();

// Returns the toggled state (0 or 1) of a given button
bool getButtonState(int which);

// Returns true if the specified button is currently being pressed
bool buttonPressed(int which);

// Clears all button toggle states (sets to 0)
void clearButtonState();

// Returns true if the specified button is being held down (long press)
// [Note: this must be implemented in the source file]
bool buttonLongPressed(int which);

// Handles specific logic based on button inputs (e.g., toggling mode)
void checkButton();

#endif // BUTTON_H