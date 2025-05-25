#include "button.h"
#include "player.h"

const int TOTAL = 5;
const int TOTAL_STATE = 2;

int buttonState[TOTAL] = {0};
int buttonP[TOTAL] = {0};  // Tracks if button is pressed
int previousButtonPressed[TOTAL] = {0};
bool longPressed[TOTAL] = {0};

// Update all button states
void updateButton() {
  for (int i = 0; i < TOTAL; i++) {
    buttonP[i] = !digitalRead(BUTTON_PIN[i]);
    if (buttonP[i] == 1 && previousButtonPressed[i] == 0) {
      buttonState[i] = (buttonState[i] + 1) % TOTAL_STATE;
    }
    previousButtonPressed[i] = buttonP[i];
  }
}

// Returns true if a specific button is currently pressed
bool buttonPressed(int which) {
  return !digitalRead(BUTTON_PIN[which]);
}

// Returns the toggled state of a specific button
bool getButtonState(int which) {
  return buttonState[which];
}

// Returns whether a button has been long pressed (not currently implemented)
bool buttonLongPressed(int which) {
  return longPressed[which];
}

// Resets all button states to 0
void clearButtonState() {
  for (int i = 0; i < TOTAL; i++) {
    buttonState[i] = 0;
  }
}

// Initializes all buttons as inputs
void setupButton() {
  for (int i = 0; i < TOTAL; i++) {
    pinMode(BUTTON_PIN[i], INPUT);
  }
}

// Example use case: checks buttons for specific actions
void checkButton() {
  // Toggle attack mode with button 3
  if (buttonPressed(3)) {
    if (getAttackMode() == YELLOW) {
      setAttackMode(BLUE);
    } else {
      setAttackMode(YELLOW);
    }
  }

  // Reflect attack mode using LED 3
  setLEDState(3, getAttackMode() == YELLOW ? 1 : 0);

  // Calibrate threshold with button 2
  if (buttonPressed(2)) {
    setLEDState(2, 1);
    calibrateThreshold();
  } else {
    setLEDState(2, 0);
  }
}
