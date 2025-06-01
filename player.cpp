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
  // === Initialization ===
  setAttackMode(YELLOW);     // Set goal to shoot on
  setRobotRole(OFFENSE);     // Set initial role
  setupVariables();

  while (1) {
    // === Core Sensor & Input Processing ===
    updateButton();
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
    // Serial.print("Speed: ");
    // Serial.print(getSpeed());
    Serial.print(" ball dist: ");
    Serial.println(getEyeValue());

    // Additional debug (uncomment to use):
    // printSensorsReading();
    // printSensorsWhite();
    // Serial.println(getLaser());
    // Serial.println(getYellowAngle());
    // Serial.println(getCompass());
    // Serial.println(hasBall());
  }
}
