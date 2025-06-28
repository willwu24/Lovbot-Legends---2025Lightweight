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
  //   setMotors(20,20,20,20);
  // }

  while (1) {
    // === Core Sensor & Input Processing ===
    updateButton();
    //displayUpdate();
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
      // goToBallPID();
      offenseMain();
    } else {
      //goToCoordinate(0,-220);
      defenseMain();
      // whiteMove(getDefenseDir());
      // setAngleThres(25);
      // setSpeed(20);
    }
    //goToCoordinate(0, 0);
    // === Actuation ===
    move();

    // === Debugging ===

    // Additional debug (uncomment to use):
    //displayCamera();
    //displayDistance();
    //displayCompass();
    // printSensorsReading();
    // printSensorsWhite();
    // Serial.println(getHomeAngle());
  }
}
