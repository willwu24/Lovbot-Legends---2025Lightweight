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
  setRobotRole(DEFENSE);     // Set initial role
  setupVariables();

  while (1) {
    // === Core Sensor & Input Processing ===
    updateButton();
    getCameraData();
    processSensors();
    processWhiteAngle();
    processDFUltraTrig();
    getCoordinate();

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
    Serial.print("Speed: ");
    Serial.println(getSpeed());
    Serial.print(" Dir: ");
    Serial.println(getDir());

    // Additional debug (uncomment to use):
    // printSensorsReading();
    // printSensorsWhite();
    // transmit(1);
    displayDistance();
    // displayCamera();
    // Serial.println(getLaser());
    // Serial.println(getYellowAngle());
    // Serial.println(getCompass());
    // Serial.println(hasBall());
  }
}
