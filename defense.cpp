#include "defense.h"

int lastWhiteDir = 180;
int lastDefenseWhiteTime = 0;
int ballTimer = 0;
int lastGoalAngle = -1;

bool gettingBall = false;
long firstGettingBall = -1;
long firstBallFront = -1;

long firstHasBall = -1;

double finalHeading;

int defenseReached = 0;
int lastDefenseReached = 0;

void defenseMain(){
  // transmit(2);
  retrieveKicker();
  if(homeDetected()){
    lastGoalAngle = getHomeAngle();
  }
  setAngleThres(30);
  if(receive()>1||defenseReached>0){
    blockGoalie();
  }
  else{
    lastDefenseReached = millis();
    defenseReached = 0;

  if(hasBall()){
    if(firstHasBall == -1){
      firstHasBall=millis();
    }
    if(lastGoalAngle<160){
      setDir(20);
    }
    else if(lastGoalAngle>200){
      setDir(340);
    }
    else{
      setDir(0);
    }
    setSpeed(90);
    if(getUltraFront()>40&&millis()-firstHasBall>300){
      kick();
      resetVariables();
    }
  }
  else if(gettingBall){
    if(firstGettingBall==-1){
      firstGettingBall = millis();
    }
    else if(millis()-firstGettingBall>600){//went for too long, go back now
      resetVariables();
    }
    else if(whiteDetected()&&millis()-firstGettingBall>300){//hit white line, go back
      resetVariables();
    }
    else{
      goToBallPID();
      setSpeed(60);
    }
  }
  else if (!whiteDetected() && millis() - lastDefenseWhiteTime < 1500){
    resetVariables();
    setSpeed(35);
    setDir(lastWhiteDir);
  }
  else if (!whiteDetected()){
    resetVariables();
    if (getUltraBack()<90){
      setDir(0);
      setTarget(0);
      setSpeed(35);
    }
    else if (homeDetected()){
      setDir(getHomeAngle());
      setTarget(0);
      setSpeed(35);
      // goToCoordinate(0,-100);
    }
    else
    {
      goToCoordinate(0,-100);
    }
  }
  else
  {
    if((getEyeAngle()<=40||getEyeAngle()>=320)&&getEyeValue()>300){
      if(firstBallFront==-1){
        firstBallFront = millis();
      }
      else if(millis()-firstBallFront>2500){
        gettingBall = true;
      }
    }
    else{
      resetVariables();
    }
    int tempCorner = (getDefenseDir() + getCompass())%360;
    if(!homeDetected()){ //on the corner white line
      Serial.print("CORNER");
      Serial.print(lastGoalAngle);
      if(lastGoalAngle<180){
        whiteMove(getWhiteMoveAngle(90));
        //setDir(45);
      }
      else{
        whiteMove(getWhiteMoveAngle(270));
        //setDir(315);
      }
      setSpeed(30);
    }
    else if((getHomeLeftEdgeAngle()>220||getHomeRightEdgeAngle()<140)&&getUltraBack()>100){

      Serial.print("EDGE");
      setDir(getHomeAngle());
      setTarget(0);
      setSpeed(40);
    }
    else{
      if (getEyeValue() < 12){
        whiteMove(getHomeDir());
        setSpeed(abs(getAngleDif(180,getHomeAngle())) * 1.0);
      }
      else if ((tempCorner > 160 && tempCorner < 190) || (tempCorner < 200 && tempCorner > 170))
      {
        whiteMove((getDefenseDir() + 180)%360);
      }
      else
      {
        whiteMove(getDefenseDir());
      }
      lastWhiteDir = getWhiteAngle();
      lastDefenseWhiteTime = millis();
    }
  }
  }
}


void whiteMove(int dir)                       // dir = 90 or 270 (robot frame)
{
  /* 1. White-line vector (robot frame) ----------------------- */
  double mag     = getMagnitude();          // 0 … 1
  double wlAngle = getWhiteAngle();         // 0 … 359 (robot frame)
  int tempCorner = (getDefenseDir() + getCompass())%360;

  double defenseTuning = min(abs(getAngleDif(180,tempCorner)), abs(getAngleDif(0,tempCorner)))/90.0;

  defenseTuning = 1.0 - defenseTuning;
  defenseTuning = constrain(defenseTuning, 0.1, 1.0);
  defenseTuning = map(defenseTuning, 0.10,1.0,0.20,2.0);

  double wx = defenseTuning * mag * sin(toRadian(wlAngle)); // X: East+
  double wy = defenseTuning * mag * cos(toRadian(wlAngle)); // Y: North+

  /* 2. Drive vector (robot frame) ---------------------------- */
  double dx = sin(toRadian(dir));           // ±1
  double dy = cos(toRadian(dir));           // 0

  /* 3. Vector addition -------------------------------------- */
  double fx = wx + dx;
  double fy = wy + dy;

  /* 4. Robot-frame heading ---------------------------------- */
  finalHeading = atan2(fx, fy) * 180.0 / PI;   // −180 … +180
  finalHeading = fmod(finalHeading + 360.0, 360.0);   // 0 … 359.999

  /* 5. Convert to field frame with compass ------------------ */
  // finalHeading = fmod(finalHeading + getCompass(), 360.0);

  setDir(finalHeading);                                // field-absolute
  
  double goalRatio = abs(getAngleDif(180, (getHomeAngle() + getCompass())%360)) / 40.0;
  goalRatio = constrain(goalRatio, 0.0, 0.8);
  goalRatio = 1.0 - goalRatio;

  double cornerRatio;
  // double cornerRatio = min(abs(getAngleDif(180,tempCorner)), abs(getAngleDif(0,tempCorner)))/90.0;
  // cornerRatio = 1.0 - cornerRatio;
  // cornerRatio = constrain(cornerRatio, 0.2, 1.0);
  // Serial.println(cornerRatio);
  // defenseTuning = map(defenseTuning, 0.2,1.0,0.2,1.0);

  if (getEyeAngle() > 180){
    cornerRatio = (270 - tempCorner) / 30.0;
  }
  else{
    cornerRatio = (tempCorner - 90) / 30.0;
  }
  cornerRatio = constrain(cornerRatio, 0.0, 1.0);
  cornerRatio = 1.0-cornerRatio;


  int defenseGoalAngle = (getHomeAngle() + getCompass())%360;

  if ((defenseGoalAngle > 180 && getEyeAngle() > 180) || (defenseGoalAngle < 180 && getEyeAngle() < 180)){
    goalRatio = 1.0;
  }

  double angleRatio = min(abs(getAngleDif(180, getEyeAngle())),abs(getAngleDif(0, getEyeAngle())))/90.0;//90.0
  // angleRatio = constrain(angleRatio, 0.0,0.6);
  angleRatio = 1.0-(exp(3.0 * (1.0 - angleRatio)) - 1.0) / (exp(3.0) - 1.0);   // high slope near 0, flat near 1
  // angleRatio = 1.0 - angleRatio;
  angleRatio = constrain(angleRatio, 0.0,1.0);


  // double distRatio = getEyeValue()/200.0;
  // distRatio = constrain(distRatio,0.8,1.0);s
  if ((tempCorner > 160 && tempCorner < 190) || (tempCorner < 200 && tempCorner > 170))
  {
    setSpeed(30);
  }
  else
  {
    double finalDefenseSpeed = angleRatio*cornerRatio*120.0;
    finalDefenseSpeed = constrain(finalDefenseSpeed,15,70);
    setSpeed(finalDefenseSpeed);
  }

  // Serial.print(angleRatio);
  // Serial.print(" ");
  // Serial.println(getSpeed());
}

bool isCorner(int angle, int CORNER_TH)
{
    static const int kCorner[4] = {45, 135, 225, 315};

    for (int i = 0; i < 4; i++)
        if (abs(getAngleDif(angle, kCorner[i]) ) < CORNER_TH)
            return true;
    return false;
}


int getDefenseDir()
{
    /* --- 1. Normalise angles -------------------------------- */
    int whiteDir = getWhiteAngle();
    if (whiteDir < 0) whiteDir += 360;
    whiteDir %= 360;

    int ballAngle = getEyeAngle();
    if (ballAngle < 0) ballAngle += 360;
    ballAngle %= 360;

    /* --- 2. Choose ±90° offset based on ball side ------------ */

    // Step 2: Choose ±90° offset based on which side of the goal-to-ball line you're on
    int ballBehind = (ballAngle + 180) % 360;

    int diff = getAngleDif(getHomeAngle(), ballBehind);  // signed angle from goal to point behind ball

    int backWhite = (getWhiteAngle()+getCompass())%360;
    if(abs(getAngleDif(backWhite,lastGoalAngle))>abs(getAngleDif(backWhite,(lastGoalAngle+180)%360))){
      backWhite=(whiteDir+180)%360;
    }
    int whiteDiff = ((getEyeAngle()+getCompass())%360-(backWhite+180)%360+360)%360; //350 - 356
    int moveAngle;
    // Serial.print(" | ");
    // Serial.print(backWhite);
    // Serial.print(" | ");
    // Serial.print(getAngleDif((whiteDir+180)%360, ballBehind));
    // Serial.print("whiteDiff: ");
    // Serial.println(whiteDiff);
    if (homeDetected()){
      if (whiteDiff < 180) {
          moveAngle = 90;   // goal is left of ballBehind → move right
      } else {
          moveAngle = 270;  // goal is right of ballBehind → move left
      }
    }
    else
    {
      if (ballAngle < 180) {
          moveAngle = 90;   // goal is left of ballBehind → move right
      } else {
          moveAngle = 270;  // goal is right of ballBehind → move left
      }
    }


    /* --- 3. Decide stripe orientation with hysteresis -------- */
    static int stripeForward = 1;          // remember last state
    const double tol = 0.1736;             // 10° dead-band
    double yComp = cos(whiteDir * M_PI / 180.0);

    if (yComp >  tol) stripeForward = 1;   // clearly forward
    if (yComp < -tol) stripeForward = 0;   // clearly backward

    /* --- 4. Preliminary heading (same formula as before) ----- */
    int defenseDir;
    if (stripeForward) {
        defenseDir = (whiteDir + 360 - moveAngle) % 360;
    } else {
        defenseDir = (whiteDir + moveAngle) % 360;
    }

    /* --- 5. Flip 180° to stand on the correct side ----------- */
    defenseDir = (defenseDir + 180) % 360;

    return defenseDir;                     // 0–359°
}

int getHomeDir(){
    /* --- 1. Normalise angles -------------------------------- */
    int whiteDir = getWhiteAngle();
    if (whiteDir < 0) whiteDir += 360;
    whiteDir %= 360;


    /* --- 2. Choose ±90° offset based on ball side ------------ */
    int moveAngle;
    if (getHomeAngle() > 180) {
        moveAngle = 270;        // ball is left  → use left offset
    } else {
        moveAngle = 90;         // ball is right → use right offset
    }

    /* --- 3. Decide stripe orientation with hysteresis -------- */
    static int stripeForward = 1;          // remember last state
    const double tol = 0.1736;             // 10° dead-band
    double yComp = cos(whiteDir * M_PI / 180.0);

    if (yComp >  tol) stripeForward = 1;   // clearly forward
    if (yComp < -tol) stripeForward = 0;   // clearly backward

    /* --- 4. Preliminary heading (same formula as before) ----- */
    int defenseDir;
    if (stripeForward) {
        defenseDir = (whiteDir + 360 - moveAngle) % 360;
    } else {
        defenseDir = (whiteDir + moveAngle) % 360;
    }

    /* --- 5. Flip 180° to stand on the correct side ----------- */
    defenseDir = (defenseDir + 180) % 360;

    return defenseDir;                     // 0–359°
}

int getWhiteMoveAngle(int moveAngle){
  int whiteDir = getWhiteAngle();
  static int stripeForward = 1;          // remember last state
  const double tol = 0.1736;             // 10° dead-band
  double yComp = cos(whiteDir * M_PI / 180.0);

  if (yComp >  tol) stripeForward = 1;   // clearly forward
  if (yComp < -tol) stripeForward = 0;   // clearly backward

  /* --- 4. Preliminary heading (same formula as before) ----- */
  int moveDir;
  if (stripeForward) {
      moveDir = (whiteDir + 360 - moveAngle) % 360;
  } else {
      moveDir = (whiteDir + moveAngle) % 360;
  }

   /* --- 5. Flip 180° to stand on the correct side ----------- */
  moveDir = (moveDir + 180) % 360;

  return moveDir;   
}

void resetVariables(){
  gettingBall = false;
  firstGettingBall = -1;
  firstBallFront = -1;
  firstHasBall = -1;
}

void blockGoalie(){
  if (defenseReached == 0){
    defenseReached = 1;
  }
  if (defenseReached == 1){
    setAngleThres(20);
    int tempOffset = (getUltraLeftSmooth() - 210)*1.5;
    setDir((0-tempOffset + 360)%360);
    int tempBlockSpeed = 80 - ((millis() - lastDefenseReached)/80);

    tempBlockSpeed = constrain(tempBlockSpeed, 30, 60);
    setSpeed(tempBlockSpeed);
    if (whiteDetected() && millis() - lastDefenseReached > 1000){
      defenseReached = 2;
    }
  }
  else if (defenseReached == 2){
    if (getMagnitude() > 0){
      setDir(0);
      setSpeed(30);
    }
    else
    {
      setDir(180);
      setSpeed(30);
    }
    
    if (getMagnitude() < 0.2 && getMagnitude() > -0.2){
      lastDefenseReached = millis();
      defenseReached = 3;
    }
  }
  else if (defenseReached == 3){
    if (getUltraLeft() < 50){
      whiteMove((getWhiteAngle() + 270)%360);
    }
    else if (getUltraRight() < 50){
      whiteMove((getWhiteAngle() + 90)%360);
    }
    else{
      setDir(STOP);
    }
    setSpeed(40);
    if (millis() - lastDefenseReached > 10000){
      defenseReached = 4;
    }
  }
  else if (defenseReached == 4){
    setDir(180);
    setSpeed(50);
    if (!whiteDetected()){
      defenseReached = 0;
    }
  }
  // if(whiteDetected()&&getUltraFront()>80){
  //   int whiteAngle = (getWhiteAngle()+getCompass())%360;
  //   if(whiteAngle<5&&whiteAngle>355){
  //     setDir(STOP);
  //   }
  //   else{
  //     if(whiteAngle>180){
  //       whiteMove(90+whiteAngle);
  //     }
  //     else{
  //       whiteMove(270+whiteAngle);
  //     }
  //   }
  // }
  // else{
  //   goToCoordinate(0, 200);
  // }

}

