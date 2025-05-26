#include "shooter.h"
#include <Arduino.h>

// === Kicker State ===
int kickerStatus = 0;
long lastKickTime = 0;
long lastRetrieveKickerTime = 0;

const int kickStrength = 90;  // Max around 90 ms
const int kickGap = 1000;     // Minimum delay between kicks
int kickCount = 0;

// === Setup ===
void setupKicker() {
  pinMode(SHOOT_PIN, OUTPUT);
}

// === Kick Control ===
void kick() {
  if (kickerStatus == OFF && millis() - lastRetrieveKickerTime > kickGap) {
    digitalWrite(SHOOT_PIN, HIGH);
    kickerStatus = ON;
    lastKickTime = millis();
  }
}

void retrieveKicker() {
  if (kickerStatus == ON && millis() - lastKickTime > kickStrength) {
    digitalWrite(SHOOT_PIN, LOW);
    lastRetrieveKickerTime = millis();
    kickerStatus = OFF;
  }
}

// === Debugging ===
void printKickerStatus() {
  Serial.print("Time: "); Serial.print(millis());
  Serial.print(" | Last Kick: "); Serial.print(lastKickTime);
  Serial.print(" | Last Retrieve: "); Serial.print(lastRetrieveKickerTime);
  Serial.print(" | Status: "); Serial.println(kickerStatus);
}
