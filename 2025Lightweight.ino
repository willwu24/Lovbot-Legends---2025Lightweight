#include <Arduino.h>
#include "func.h"
#include "player.h"
#include "compass.h"
#include "led.h"
#include "button.h"
#include "camera.h"
#include "dribbler.h"
#include "shooter.h"
#include "Grayscale.h"
#include "bluetooth.h"
#include "goToBall.h"
#include "compoundEye.h"
#include "DFUltraTrig.h"
//button 1-6 = digital 30-35


void setup() {
  setSerialMonitor(); 
  Serial.print("Start");
  setupButton();

  while(!buttonPressed(4)) //Press button 5 to start the program
  {
    setLEDState(4,0);
    updateButton();
  }
  setLEDState(4,1);

  Serial.println("Running basic setup...");
  setBotNum(1);
  setupMotors();

  Serial.println("Setting up sensors...");
  
  setupKicker();
  setupCamera();
  setupCompass();
  setupBluetooth();
  initSensors();
  eyeSetup();
  setUpBallPID();
  displaySetup();

  Serial.println("Finished setup.");
}

void loop() {
  player();
}
