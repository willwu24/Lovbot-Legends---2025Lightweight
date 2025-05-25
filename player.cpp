#include "player.h"

int kickoffTimer = 0;
int tempKickoff = 0;

//..........................................................................................
//....................lllll.................................................................
//...................pllllll................................................................
//...................pllllll................................................................
//...................pllllll................................................................
//...................pllllll................................................................
//...................pllllll................................................................
//.ppppppppppppp.....pllllll....aaaaaaaaaa...aayyyy.....yyyyyy....eeeeeeeee.....rrrrr.rrrr..
//.ppppppppppppppp...pllllll...aaaaaaaaaaaa..aayyyyy....yyyyyy...eeeeeeeeeee....rrrrrrrrrr..
//.pppppppppppppppp..pllllll..aaaaaaaaaaaaaa.aayyyyy...yyyyyyy.yeeeeeeeeeeeee...rrrrrrrrrr..
//.pppppppppppppppp..pllllll..aaaaaaaaaaaaaaaaayyyyy...yyyyyyy.yeeeeeeeeeeeeee..rrrrrrrrrr..
//.ppppppppppppppppp.pllllll.laaaaaaaaaaaaaaa.ayyyyy...yyyyyy.yyeeeeeeeeeeeeee..rrrrrrrrrr..
//.ppppppp...ppppppp.pllllll.laaaaa...aaaaaaa.ayyyyyy..yyyyyy.yyeeeee...eeeeeee.rrrrrrrr....
//.pppppp....ppppppp.pllllll..aaaaaaaaaaaaaaa.ayyyyyy.yyyyyyy.yyeeeeeeeeeeeeeee.rrrrrr......
//.pppppp.....pppppp.pllllll...aaaaaaaaaaaaaa..yyyyyy.yyyyyy..yyeeeeeeeeeeeeeee.rrrrrr......
//.pppppp.....pppppp.pllllll..aaaaaaaaaaaaaaa..yyyyyyyyyyyyy..yyeeeeeeeeeeeeeee.rrrrrr......
//.pppppp.....pppppp.pllllll.laaaaaaaaaaaaaaa...yyyyyyyyyyyy..yyeeeeeeeeeeeeeee.rrrrrr......
//.pppppp.....pppppp.pllllll.laaaaaaaa.aaaaaa...yyyyyyyyyyy...yyeeeeeeeeeeeeeee.rrrrrr......
//.pppppp....ppppppp.pllllll.laaaaa...aaaaaaa...yyyyyyyyyyy...yyeeee.....eeeee..rrrrrr......
//.ppppppp..pppppppp.pllllll.laaaaa...aaaaaaa....yyyyyyyyy....yyeeeee...eeeeeee.rrrrrr......
//.pppppppppppppppp..pllllll.laaaaaaaaaaaaaaa....yyyyyyyyy.....yeeeeeeeeeeeeee..rrrrrr......
//.pppppppppppppppp..pllllll.laaaaaaaaaaaaaaa....yyyyyyyyy.....yeeeeeeeeeeeeee..rrrrrr......
//.ppppppppppppppp...pllllll.laaaaaaaaaaaaaaa.....yyyyyyy.......eeeeeeeeeeeee...rrrrrr......
//.pppppppppppppp....pllllll..aaaaaaaaaaaaaaaa....yyyyyyy........eeeeeeeeeee....rrrrrr......
//.pppppp.pppppp................aaaaaa............yyyyyyy..........eeeeeee..................
//.pppppp.........................................yyyyyy....................................
//.pppppp......................................yyyyyyyyy....................................
//.pppppp......................................yyyyyyyyy....................................
//.pppppp......................................yyyyyyyy.....................................
//.pppppp......................................yyyyyyy......................................
//.pppppp......................................yyyyyy.......................................
//..........................................................................................


/*
NOTE:
HAS BALL SET TO ALWAYS FALSE
BLOCKING REMOVED
NO PARKING FLAG
*/

/*
  Mode Offence_Switch -> Role Offence
  Mode Defense_Switch -> Role Defense
  Mode Default_Switch -> ???
*/

void player() {
  //Set which goal you are shooting on
  setAttackMode(YELLOW);
  //Set offense or defense;
  setRobotRole(OFFENSE);
  setupVariables();
  
  // while(1){
  //   setMotors(20,20,20,20);
  //   //setMotor(4, 20);
  // }
  while (1) {
    //Basic Checks; include in all programs
    updateButton();
    getCameraData();
    // Serial.println(analogRead(A6));
    processSensors();
    processWhiteAngle();
    processDFUltraTrig();
    getCoordinate();
    odoUpdate();
    // printSensorsReading();
    setLED();
    //Change Button Logic
    checkButton();

    //Change LED Logic
    checkLED();
    //transmit(4);
    //bluetoothMain();
    //Finished all checks
    // OFFENSE
    // Serial.println(getLaser());
    if (getRobotRole() == OFFENSE) {
      //Serial.print("OFFENSE");
      offenseMain();
    }
    //DEFENSE
    else {
      defenseMain();
    }
    displayDistance();
    displayCoordinate();
    Serial.print("dir: ");
    Serial.print(getDir());
    Serial.print("getEyeAngle: ");
    Serial.print(getEyeAngle());
    Serial.print(" ");
    printOdo();
    Serial.println();
    move();
  }
}
