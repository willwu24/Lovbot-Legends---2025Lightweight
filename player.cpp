#include "player.h"


/*
  NOTE:
  - Blocking is removed
  - No parking flag implemented

  Mode Offense_Switch  → Role Offense
  Mode Defense_Switch  → Role Defense
  Mode Default_Switch  → Unknown
*/

void player() {
  Serial.print("Player");
  // === Initialization ===
  setAttackMode(YELLOW);     // Set goal to shoot on
  setRobotRole(DEFENSE);     // Set initial role
  setupVariables();
  // while(1){
  //   setMotors(20,20,20,20);
  // }

  while (1) {
    // === Core Sensor & Input Processing ===
    updateButton();
    displayUpdate();
    getCameraData();
    processSensors();
    processWhiteAngle();
    processDFUltraTrig();
    getCoordinate();

    //printSensorsThres();
    // Serial.println(" ");

    setLED();
    checkButton();
    checkLED();

    // === Role Handling ===
    if (getRobotRole() == OFFENSE) {
      offenseMain();
    } else {
      defenseMain();
    }

    // === Actuation ===
    move();

    // === Debugging ===
    // Serial.print("Dir: ");
    // Serial.print(getDir());
    // Serial.print(" Speed: ");
    // Serial.println(getSpeed());
    Serial.println(getHomeAngle());

    // Additional debug (uncomment to use):
    //displayCamera();
    //displayDistance();
    //displayCompass();
    // printSensorsReading();
    // printSensorsWhite();
    // Serial.println(getLaser());
    // Serial.println(getYellowAngle());
    // Serial.println(getCompass());
    // Serial.println(hasBall());
  }
}
