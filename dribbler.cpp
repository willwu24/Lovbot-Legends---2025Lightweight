#include "dribbler.h"

int dribblerState=OFF;
int dribblerSpeed=LOW;
int previousState=dribblerState;
int previousSpeed=dribblerSpeed;
long lastLowTime=-1000;
long switchOffReactionTime=100;
long lastSwitchOnTime=-switchOffReactionTime;
Servo ESC;

void setupDribbler(){//a setup function to make sure the dribbler is functioning
  Serial.println("setting up dribbler");
  ESC.attach(DRIBBLER_PIN_1); // (pin, min pulse width, max pulse width in microseconds) 
  long start = millis();
  ESC.write(30);
  delay(1000);
}


void setDribbler(int speed, int status){
  ESC.write(speed);
}

bool getDribblerSpeed(){
  return dribblerSpeed;
}

bool getDribblerState(){
  return dribblerState;
}

long getLastLowTime(){
  return lastLowTime;
}

void setLastLowTime(){
  lastLowTime=millis();
}
