#include <Arduino.h>
#include "func.h"
#include "player.h"
#include "compass.h"
#include "distance.h"
#include "led.h"
#include "button.h"
#include "camera.h"
#include "dribbler.h"
#include "shooter.h"
#include "Grayscale.h"
#include "GoToCoordinate.h"
#include "bluetooth.h"
#include "goToBall.h"
#include "compoundEye.h"
#include "DFUltraTrig.h"
//button 1-6 = digital 30-35


void setup() {

  setupButton();

  while(!buttonPressed(4)) //Press button 5 to start the program
  {
    setLEDState(4,0);
    updateButton();
  }
  setLEDState(4,1);

  Serial.println("Running basic setup...");
  setSerialMonitor(); 
  setBotNum(1);
  setupMotors();

  Serial.println("Setting up sensors...");
  setupKicker();
  setupCamera();
  setupCompass();
  setupBluetooth();
  initSensors();
  eyeSetup();
  
  Serial.println("Finished setup.");
}

void loop() {
  player();
}
