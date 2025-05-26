#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "func.h"

// Initializes or updates the LED outputs based on current state
void setLED();

// Sets the state (OFF, ON, BLINK) of a specific LED
void setLEDState(int which, int state);

// Turns off all LEDs
void clearLEDState();

// Sets all LEDs to a specific state (OFF, ON, BLINK)
void setLEDAll(int state);

// Updates LEDs based on game state (e.g., kickoff or line detected)
void checkLED();

#endif // LED_H
