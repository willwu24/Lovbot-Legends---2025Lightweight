#include "shooter.h"
#include <Arduino.h>


int kickerStatus = 0;

long lastKickTime = 0;
long lastRetrieveKickerTime = 0;

int kickStrength = 90; //max is around 90
int kickGap = 1000;
int kickCount = 0;

void setupKicker() {
  pinMode(SHOOT_PIN, OUTPUT);

}

void kick(){
  if (kickerStatus == OFF){
    if (millis() - lastRetrieveKickerTime > kickGap){
      digitalWrite(SHOOT_PIN, HIGH);
      kickerStatus = ON;
      lastKickTime = millis();
    }
  }
}

void retrieveKicker(){
  if (kickerStatus == ON){
    if (millis() - lastKickTime > kickStrength){
      digitalWrite(SHOOT_PIN, LOW);
      lastRetrieveKickerTime = millis();
      kickerStatus = OFF;
    }
  }
}

void printKickerStatus(){
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(lastKickTime);
  Serial.print(" ");
  Serial.print(lastRetrieveKickerTime);
  Serial.print(" ");
  Serial.println(kickerStatus);
}