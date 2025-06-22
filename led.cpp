#include "led.h"
#include "player.h"

// Constants
const int TOTAL_LED = 5;
const int BLINK_INTERVAL = 300; // milliseconds

// LED state tracking
int LEDStates[TOTAL_LED] = {0, 0, 0, 0, 0};
int LEDPins[TOTAL_LED] = {33, 34, 35, 36, 37};
long blinkTime[TOTAL_LED] = {0, 0, 0, 0, 0};

// Update the physical LED state based on LEDStates array
void setLED() {
  long now = millis();

  for (int i = 0; i < TOTAL_LED; ++i) {
    switch (LEDStates[i]) {
      case OFF:
        digitalWrite(LEDPins[i], LOW);
        break;

      case ON:
        digitalWrite(LEDPins[i], HIGH);
        break;

      case BLINK:
        if (now - abs(blinkTime[i]) > BLINK_INTERVAL) {
          if (blinkTime[i] > 0) {
            blinkTime[i] = -now;
            digitalWrite(LEDPins[i], LOW);
          } else {
            blinkTime[i] = now;
            digitalWrite(LEDPins[i], HIGH);
          }
        }
        break;
    }
  }
}

// Set individual LED state
void setLEDState(int which, int state) {
  if (which >= 0 && which < TOTAL_LED && state >= 0 && state <= 2) {
    LEDStates[which] = state;
  }
}

// Set all LEDs to a specified state
void setLEDAll(int state) {
  for (int i = 0; i < TOTAL_LED; ++i) {
    LEDStates[i] = state;
    if (state == BLINK) {
      blinkTime[i] = millis();
    }
  }
  setLED();
}

// Turn off all LEDs
void clearLEDState() {
  setLEDAll(OFF);
}

// Automatically control LED indicators based on game state
void checkLED() {
  // setLEDState(4, getKickoff() ? ON : OFF);          // LED 4 for kickoff
  setLEDState(0, whiteDetected() ? ON : OFF);       // LED 0 for white line detection
  // Additional indicators can be added here
}
