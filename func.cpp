#include "func.h"

// PID variables

double motorCorrectionKp = 0.18, motorCorrectionKi = 0.00, motorCorrectionKd = 0.02;
double motorTurningKp = 0.16, motorTurningKi = 0.00, motorTurningKd = 0.00;
double motorSetpoint, motorInput, motorCorrectionOutput, motorTurningOutput;

PID motorCorrectionPID(&motorInput, &motorCorrectionOutput, &motorSetpoint, motorCorrectionKp, motorCorrectionKi, motorCorrectionKd, DIRECT);
PID motorTurningPID(&motorInput, &motorTurningOutput, &motorSetpoint, motorTurningKp, motorTurningKi, motorTurningKd, DIRECT);

// Motion control variables

int angleThres = 35;
int turnSpeed = 7;
double motorChange = 3;
int turningMode = 1;
int motorMode = 0;
int MIN_TURNING_SPEED = 6;
int MAX_TURNING_SPEED = 25;

int currSpeed[5] = {0};
int tarSpeed[5] = {0};
unsigned long lastChanged = 0;

// Bot state variables

int attackMode;
int robotRole;
int robotMode;
bool kickoff = false;
int botNum = 1;
int targetMode = 1;

long lastLaser = 0;
bool laserSensed = false;
int laserTimer = -10000;
int laserCounter = -1;
int laserTolerance = 80;

moveType motion;

int motorSign[4][4] = {
  { 1,  1, -1, -1},
  {-1,  1,  1, -1},
  {-1, -1,  1,  1},
  { 1, -1, -1,  1}
};

//---------------------------------------------------------------- SETUP FUNCTIONS

void setSerialMonitor() {
  Serial.begin(9600);
}

void setMotorsDir(int dir1, int dir2, int dir3, int dir4) {
  motor_dir[1] = dir1;
  motor_dir[2] = dir2;
  motor_dir[3] = dir3;
  motor_dir[4] = dir4;
}

void setupMotors() {
  Serial.println("setup motors");
  for (int i = 1; i <= 4; i++) {
    pinMode(PWM[i], OUTPUT);
    pinMode(DIR[i], OUTPUT);
  }
  motorCorrectionPID.SetMode(AUTOMATIC);
  motorCorrectionPID.SetOutputLimits(-45, 45);
  motorTurningPID.SetMode(AUTOMATIC);
  motorTurningPID.SetOutputLimits(-45, 45);
  motorSetpoint = 0;
}

void setAngleThres(int angle) { angleThres = angle; }
int getAngleThres() { return angleThres; }
void setTurnSpeed(int s) { turnSpeed = s; }

//---------------------------------------------------------------- MOTION FUNCTIONS

int getTarget() { return motion.target; }
void setTarget(int newTarget) { motion.target = newTarget; }
int getSpeed() { return motion.speed; }
void setSpeed(int newSpeed) { motion.speed = newSpeed; }
int getDir() { return motion.direction; }
void setDir(int newDirection) { motion.direction = newDirection; }

void setTurningMode(int mode) { turningMode = mode; }
int getTurningMode() { return turningMode; }
void setMotorMode(int mode) { motorMode = mode; }
int getMotorMode() { return motorMode; }
void setMotorChange(int change) { motorChange = change; }
double getMotorChange() { return motorChange; }

//---------------------------------------------------------------- MOVEMENT

void move(){
  int d = motion.direction;
  int s = motion.speed;
  int targetAngle = motion.target;
  int degree1;
  int degree2;
  int slowerSpeed = s;
  int speed1;
  int speed2;
  int speed3;
  int speed4;

  int angleDif = getAngleDif(getTarget(),getCompass());

  int refLine = 50 + 90*(d / 90) - 10*((d/90)%2);   
  int refAngle = abs(refLine - d);
  int sineAngle = 90+10*((d-refLine)/refAngle)*(pow(-1,(d/90))); 
  int faster = sinLaw(180-refAngle-sineAngle,sineAngle,s);
  int slower = sinLaw(refAngle,sineAngle,s);

  int speedCombination = (d / 90)%2;
  int motorSpeedComb [2][4] = {{slower, faster, slower, faster}, {faster, slower, faster, slower}};

  int shiftedDirection = (d + 50) % 360;
  int signCombination;

  // PID
  motorInput = angleDif;
  motorCorrectionPID.Compute();
  motorTurningPID.Compute();


  if(shiftedDirection<90){
    signCombination=0;
  }
  else if(shiftedDirection<180){
    signCombination=1;
  }
  else if(shiftedDirection<270){
    signCombination=2;
  }
  else{
    signCombination=3;
  }

  speed1 = motorSign[signCombination][0] * motorSpeedComb[speedCombination][0];
  speed2 = motorSign[signCombination][1] * motorSpeedComb[speedCombination][1];
  speed3 = motorSign[signCombination][2] * motorSpeedComb[speedCombination][2];
  speed4 = motorSign[signCombination][3] * motorSpeedComb[speedCombination][3];
  if(d == STOP){//both situation stops
    speed1 = 0;
    speed2 = 0;
    speed3 = 0;
    speed4 = 0;
  }
  
  // now check mode of turning speed (manual or PID)
  if (turningMode == MANUAL) {
    // manual mode
    if (abs(angleDif)>angleThres){//have to turn
      if (angleDif<0){
        //turn clockwise
        speed1=turnSpeed;
        speed2=turnSpeed;
        speed3=turnSpeed;
        speed4=turnSpeed;
      }
      else{
        //turn counter-clockwise
        speed1=-turnSpeed;
        speed2=-turnSpeed;
        speed3=-turnSpeed;
        speed4=-turnSpeed;
      }
    }
    else if (abs(angleDif)>10){
      if(d==STOP){
        if (angleDif<0){
          speed1 = turnSpeed;
          speed2 = turnSpeed;
          speed3 = turnSpeed;
          speed4 = turnSpeed;
        }
        else{
          speed1 = -turnSpeed;
          speed2 = -turnSpeed;
          speed3 = -turnSpeed;
          speed4 = -turnSpeed;
        }
      }
      else{
        //correction without turning
        speed1-=-motorCorrectionOutput;
        speed2-=-motorCorrectionOutput;
        speed3-=-motorCorrectionOutput;
        speed4-=-motorCorrectionOutput;
      }
    }
  }
  else { // PID mode
    if (abs(angleDif)>angleThres){//have to turn
      if (angleDif<0){
        //turn clockwise
        motorTurningOutput = constrain(motorTurningOutput, MIN_TURNING_SPEED, MAX_TURNING_SPEED);
        speed1=motorTurningOutput;
        speed2=motorTurningOutput;
        speed3=motorTurningOutput;
        speed4=motorTurningOutput;
      }
      else{
        //turn counter-clockwise
        motorTurningOutput = constrain(motorTurningOutput, -MAX_TURNING_SPEED, -MIN_TURNING_SPEED);
        speed1=motorTurningOutput;
        speed2=motorTurningOutput;
        speed3=motorTurningOutput;
        speed4=motorTurningOutput;
      }
    }
    else if (abs(angleDif)>10){
      if(d==STOP){
        if (angleDif<0){
          motorTurningOutput = constrain(motorTurningOutput, MIN_TURNING_SPEED, MAX_TURNING_SPEED);
          speed1 = motorTurningOutput;
          speed2 = motorTurningOutput;
          speed3 = motorTurningOutput;
          speed4 = motorTurningOutput;
        }
        else{
          motorTurningOutput = constrain(motorTurningOutput, -MAX_TURNING_SPEED, -MIN_TURNING_SPEED);
          speed1 = motorTurningOutput;
          speed2 = motorTurningOutput;
          speed3 = motorTurningOutput;
          speed4 = motorTurningOutput;
        }
      }
      else{
        //correction without turning
        speed1-=-motorCorrectionOutput;
        speed2-=-motorCorrectionOutput;
        speed3-=-motorCorrectionOutput;
        speed4-=-motorCorrectionOutput;
      }
    }
  }
  if(motorMode == 0){
    setMotors(speed1,speed2,speed3,speed4);
  }
  else{
    setMotorsGradual(speed1,speed2,speed3,speed4);
  }
  // Serial.print(motorCorrectionOutput);
}


//---------------------------------------------------------------- MOTOR HELPERS

double toRadian(int degree) { return degree * 3.1415926 / 180; }
double toDegree(double radian) { return radian * 180 / 3.14159265; }

int sinLaw(int degree1, int degree2, int speed2) {
  double rad1 = toRadian(degree1);
  double rad2 = toRadian(degree2);
  return (sin(rad1) * speed2) / sin(rad2);
}

int getAngleDif(int target, int currentValue) {
  int current = currentValue;
  if (target < 180) {
    return current > target + 180 ? -360 + current - target : current - target;
  } else {
    return current > target - 180 ? current - target : 360 - target + current;
  }
}

int getAbsAngle(int a) {
  a = abs(a) % 360;
  return a > 180 ? 360 - a : a;
}

void setMotor(int which, int motorSpeed) {
  motorSpeed *= motor_dir[which];
  motorSpeed = constrain(motorSpeed, -MAX_SPEED, MAX_SPEED);

  digitalWrite(DIR[which], motorSpeed >= 0 ? LOW : HIGH);
  analogWrite(PWM[which], motorSpeed >= 0 || motor_prob[which] == 1 ? abs(motorSpeed) * 255 / 100 : 255 + motorSpeed * 255 / 100);
}

void setMotors(int s1, int s2, int s3, int s4) {
  setMotor(1, s1);
  setMotor(2, s2);
  setMotor(3, s3);
  setMotor(4, s4);
}

void setMotorsGradual(int s1, int s2, int s3, int s4) {
  tarSpeed[1] = s1;
  tarSpeed[2] = s2;
  tarSpeed[3] = s3;
  tarSpeed[4] = s4;

  if (millis() - lastChanged > 1) {
    for (int i = 1; i <= 4; i++) {
      if (abs(tarSpeed[i] - currSpeed[i]) < motorChange) {
        currSpeed[i] = tarSpeed[i];
      } else {
        currSpeed[i] += (tarSpeed[i] - currSpeed[i]) / abs(tarSpeed[i] - currSpeed[i]) * motorChange;
      }
    }
    lastChanged = millis();
  }

  setMotors(currSpeed[1], currSpeed[2], currSpeed[3], currSpeed[4]);
}

void STOPNOW() {
  setMotors(0, 0, 0, 0);
}

//---------------------------------------------------------------- LASER & BALL

bool getLaser() {
  return analogRead(LAZER_PIN) < 10;
}

bool hasBall() {
  if (getLaser() && (getEyeAngle() < 60 || getEyeAngle() > 300)) {
    laserCounter++;
    lastLaser = millis();
    return true;
    // if (getBotNum() == 1 || laserCounter > 2) return true;
    // return false;
  } else {
    if(millis()-lastLaser<50){
      return true;
    }
    laserCounter = -1;
    return false;
  }
}

//---------------------------------------------------------------- TIMER

long getSystemTime() { return millis(); }
void resetSystemTime() {}
void wait(float sec) { delay(sec * 1000); }

//---------------------------------------------------------------- STATE ACCESSORS

void setAttackMode(int a) { attackMode = a; }
int getAttackMode() { return attackMode; }

void setRobotRole(int a) { robotRole = a; }
int getRobotRole() { return robotRole; }

void setRobotMode(int a) { robotMode = a; }
int getRobotMode() { return robotMode; }

int getTargetMode() { return targetMode; }
void setTargetMode(int a) { targetMode = a; }

bool getKickoff() { return kickoff; }
void setKickoff(bool a) { kickoff = a; }

void setBotNum(int a) { EEPROM.write(32, a); }
int getBotNum() { return (EEPROM.read(32)); }
