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
  setAttackMode(BLUE);     // Set goal to shoot on
  setRobotRole(OFFENSE);     // Set initial role
  setupVariables();
  // while(1){
  //   setMotors(20,20,-20,-20);
  // }

  while (1) {
    // === Core Sensor & Input Processing ===
    updateButton();
    displayUpdate();
    getCameraData();
    processSensors();
    processDFUltraTrig();
    getCoordinate();
    transmitData();
    receiveData();

    //printSensorsThres();
    // Serial.println(" ");

    setLED();
    checkButton();
    checkLED();

    // === Role Handling ===
    if (getRobotRole() == OFFENSE) {
      processWhiteAngleOffense();
      offenseMain();
    } else {
      processWhiteAngle();
      // defenseMain();
      defenseMain();
      // setMotors(20,20,20,20);
    }
    //goToCoordinate(0, 0);
    // === Actuation ===

    move();

    // === Debugging ===
    // Serial.print(getSendArr(0));
    // Serial.print(' ');
    // Serial.println(getReceiveArr(0));
    // Additional debug (uncomment to use):

  }
}
