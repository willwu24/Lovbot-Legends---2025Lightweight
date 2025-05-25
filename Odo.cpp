#include "Odo.h"
#include <Wire.h>

// Define global OTOS object
QwiicOTOS myOtos;
sfe_otos_pose2d_t position;
int odoPosX;
int odoPosY;
int odoPosHeading;

void odoSetup() {
    Serial.begin(9600);
    Wire.begin();

    // Attempt to initialize the OTOS sensor
    while (!myOtos.begin()) {
        Serial.println("OTOS not connected, check your wiring and I2C address!");
        delay(1000);
    }

    Serial.println("Ensure the OTOS is flat and stationary, then enter any key to calibrate the IMU");

    // Flush serial buffer
    while (Serial.available()) Serial.read();
//    while (!Serial.available());

    

    // Set units (can modify to suit project)
   calibrateOdo();
}

void calibrateOdo(){
  myOtos.setLinearUnit(kSfeOtosLinearUnitMeters);        // or kSfeOtosLinearUnitInches
  myOtos.setAngularUnit(kSfeOtosAngularUnitDegrees);     // or kSfeOtosAngularUnitRadians
  
  // Reset tracking
  myOtos.resetTracking();
  Serial.println("Calibrating IMU...");
  myOtos.calibrateImu();
  myOtos.resetTracking();

}

void printOdo(){
  Serial.println();
  Serial.println("Position:");
  Serial.print("X (cm): ");
  Serial.println(odoPosX);  // Convert meters to centimeters
  Serial.print("Y (cm): ");
  Serial.println(odoPosY);
  Serial.print("Heading (degree): ");
  Serial.println(odoPosHeading);
}

int getOdoX(){
  return odoPosX;
}

int getOdoY(){
  return odoPosY;
}

int getOdoHeading(){
  return odoPosHeading;
}

int getCompass(){
  int rawDegree = getOdoHeading();
  if(buttonPressed(1)){
    calibrateOdo();
  }
  return rawDegree;
  // int calculated = (int) (rawDegree - heading + 360)%360;
  // return calculated;
}


void odoUpdate() {
    myOtos.getPosition(position);
    odoPosX = position.x*-100;
    odoPosY = position.y*-100;
    odoPosHeading = (-int(position.h)+360)%360;
    

}
