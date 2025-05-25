#include "dribbler.h"

// Dribbler state constants
int dribblerState = OFF;
int dribblerSpeed = LOW;

int previousState = dribblerState;
int previousSpeed = dribblerSpeed;

long lastLowTime = -1000;
const long switchOffReactionTime = 100;
long lastSwitchOnTime = -switchOffReactionTime;

Servo ESC;

void setupDribbler() {
  Serial.println("Setting up dribbler...");
  ESC.attach(DRIBBLER_PIN_1); // Attach ESC to specified pin
  ESC.write(30);              // Initial speed pulse to activate ESC
  delay(1000);                // Give time for ESC to initialize
}

void setDribbler(int speed, int status) {
  ESC.write(speed);
  dribblerSpeed = speed;
  dribblerState = status;
}

bool getDribblerSpeed() {
  return dribblerSpeed;
}

bool getDribblerState() {
  return dribblerState;
}

long getLastLowTime() {
  return lastLowTime;
}

void setLastLowTime() {
  lastLowTime = millis();
}
