
#include "func.h"

//variable declearation

double motorCorrectionKp=0.50, motorCorrectionKi=0.00, motorCorrectionKd=0.00;
double motorTurningKp=0.15, motorTurningKi=0.00, motorTurningKd=0.00;
// double motorKp=0.21, motorKi=0.00, motorKd=0.012;
double motorSetpoint, motorInput, motorCorrectionOutput, motorTurningOutput;

PID motorCorrectionPID(&motorInput, &motorCorrectionOutput, &motorSetpoint, motorCorrectionKp, motorCorrectionKi, motorCorrectionKd, DIRECT);
PID motorTurningPID(&motorInput, &motorTurningOutput, &motorSetpoint, motorTurningKp, motorTurningKi, motorTurningKd, DIRECT);


// //int motor_dir[5] = {0,-1,-1,-1,-1};// my bot
//int motor_dir[5] = {0,-1,1,-1,-1};//robert bot
int angleThres = 35;
int turnSpeed = 7;
double motorChange = 3;
int turningMode = 0;
int motorMode = 0;//0: immediate| 1: gradual
int MIN_TURNING_SPEED = 6;
int MAX_TURNING_SPEED = 25;
int lastLaser = 0;
bool kickoff = false;
bool laserSensed = false;
int laserTimer = -10000;

int laserCounter = -1;
int laserTolerance = 80;

int attackMode; //yellow = 0 , blue = 1;
int robotRole;
int robotMode;
int botNum; // 0: my bot, 1: robert bot
moveType motion;

int targetMode = 1;

int currSpeed[5] = {0, 0, 0, 0, 0};
int tarSpeed[5] = {0, 0, 0, 0, 0};
unsigned long lastChanged = 0;

//----------------------------------------------------------------setup functions
void setSerialMonitor(){
  Serial.begin(9600);
}



//input the four constants for motors. only input 1 or -1
void setMotorsDir(int dir1, int dir2, int dir3, int dir4){
	motor_dir[1] = dir1;
	motor_dir[2] = dir2;
	motor_dir[3] = dir3;
	motor_dir[4] = dir4;
}

void setupMotors(){
  //do not make any modification!!!
  //setup the pin of the motor
  Serial.println("setup motors");
  pinMode(PWM[1],OUTPUT);
  pinMode(PWM[2],OUTPUT);
  pinMode(DIR[1],OUTPUT);
  pinMode(DIR[2],OUTPUT);
  pinMode(PWM[3],OUTPUT);
  pinMode(PWM[4],OUTPUT);
  pinMode(DIR[3],OUTPUT);
  pinMode(DIR[4],OUTPUT);
  // TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz

  // PID
  motorCorrectionPID.SetMode(AUTOMATIC);
  motorCorrectionPID.SetOutputLimits(-45, 45);
  motorTurningPID.SetMode(AUTOMATIC);
  motorTurningPID.SetOutputLimits(-45, 45);
  motorSetpoint = 0;
}
void setAngleThres(int angle){
  angleThres = angle;
}
int getAngleThres(){
  return angleThres;
}
void setTurnSpeed(int s){
  turnSpeed = s;
}

//----------------------------------------------------------------sensors






//----------------------------------------------------------------Motion functions

int getTarget() {
	return motion.target;
}
void setTarget(int newTarget){
	motion.target = newTarget;
}

int getSpeed() {
	return motion.speed;
}

void setSpeed(int newSpeed){
	motion.speed = newSpeed;
}

int getDir() {
	return motion.direction;
}

void setDir(int newDirection){
	motion.direction = newDirection;
}

void setTurningMode(int mode) {
  turningMode = mode;
  // mode 0 is manual (no PID)
  // mode 1 is PID
}

int getTurningMode() {
  return turningMode;
}

void setMotorMode(int mode) {
  motorMode = mode;
  // mode 0 is instant
  // mode 1 is gradual
}

int getMotorMode() {
  return motorMode;
}

void setMotorChange(int change) {
  motorChange = change;
  // mode 0 is instant
  // mode 1 is gradual
}

double getMotorChange() {
  return motorChange;
}

//----------------------------------------------------------------motors

#define alpha 50
int motorSign [4][4] = {{1, 1, -1, -1}, {-1, 1, 1, -1}, {-1, -1, 1, 1}, {1, -1, -1, 1}};

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

  
  // int angleDif = getAngleDif(getTarget());

  // int refLine = 45 + 90*(d / 90) - 10*((d/90)%2);   
  // int refAngle = abs(refLine - d);
  // int sineAngle = 90+10*((d-refLine)/refAngle)*(pow(-1,(d/90))); 
  // int faster = sinLaw(180-refAngle-sineAngle,sineAngle,s);
  // int slower = sinLaw(refAngle,sineAngle,s);

  // int speedCombination = (d / 90)%2;
  // int motorSpeedComb [2][4] = {{slower, faster, slower, faster}, {faster, slower, faster, slower}};

  // int shiftedDirection = (d + 45) % 360;
  // int signCombination;

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


//----------------------------------------------------------------Move helper

double toRadian(int degree){
  /*intake a degree;
   *return the degree in radian;
   */
  return degree*3.1415926/180;
}

double toDegree(double radian){
  return radian * 180 / 3.14159265;
}

int sinLaw(int degree1,int degree2,int speed2){
  /*intake both angle and one of the length
   *output the other length
  */
    double radian1 = toRadian(degree1);
    double radian2 = toRadian(degree2);
    return (sin(radian1)*speed2)/sin(radian2);
}


int getAngleDif(int target, int currentValue){
  int output = 0;
  int current = currentValue;
  if(target<180){
    if(current>target+180){
      output = -360+current-target;
    }
    else{
      output = current-target;
    }
  }
  else{
    if(current>target-180){
      output = current-target;
    }
    else{
      output = 360-target+current;
    }
  }

  return output;
}

int getAbsAngle(int a) {
  a = abs(a);
  a%=360;
  if (a > 180) {
    return 360 - a;
  } else {
    return a;
  }
}

void setMotor(int which,int motorSpeed){
  //set one motor
  //do not make any modification!!!


  
  motorSpeed*=motor_dir[which];

  //protect the motor not to over-speed
  if(motorSpeed>MAX_SPEED){
    motorSpeed = MAX_SPEED;
  }
  else if(motorSpeed<-MAX_SPEED){
    motorSpeed = -MAX_SPEED;
  }
  
  digitalWrite(DIR[which],motorSpeed>=0?LOW:HIGH);//
  analogWrite(PWM[which],motorSpeed>=0||motor_prob[which]==1?abs(motorSpeed)*255/100:255+motorSpeed*255/100);

}

void setMotors(int speed1, int speed2, int speed3, int speed4){
  setMotor(1,speed1);
  setMotor(2,speed2);
  setMotor(3,speed3);
  setMotor(4,speed4);
}

void setMotorsGradual(int speed1, int speed2, int speed3, int speed4){
  tarSpeed[1] = speed1;
  tarSpeed[2] = speed2;
  tarSpeed[3] = speed3;
  tarSpeed[4] = speed4;
  if(millis()-lastChanged>1){
    for(int i = 1; i<=4; i++){
      if(abs(tarSpeed[i]-currSpeed[i])<motorChange){
        currSpeed[i] = tarSpeed[i];
      }
      else{
        currSpeed[i]+=(tarSpeed[i]-currSpeed[i])/abs((tarSpeed[i]-currSpeed[i]))*motorChange;
      }
    }
    lastChanged = millis();
  }
  
  setMotor(1,currSpeed[1]);
  setMotor(2,currSpeed[2]);
  setMotor(3,currSpeed[3]);
  setMotor(4,currSpeed[4]);
}


void STOPNOW(){
  setMotors(0,0,0,0);
}







//----------------------------------------------------------------KICKER
void setKicker(int state){
  digitalWrite(SHOOT_PIN,state);
}

bool getLaser(){
  int analogReading = analogRead(LAZER_PIN);
  return analogReading < 3;
}

bool hasBall() {
  if(getLaser()==true&&(getEyeAngle()<40||getEyeAngle()>320)){//laser detects true and ball in front
    laserCounter+=1;
    if(getBotNum()==1){//bot 1, laser not as sensitive
      return true;
    }
    else if(laserCounter>5){//bot 0, laser sometimes gives false positive, need to filter
      // laserCounter = -1;
      return true;
    }
    else{
      return false;
    }
  }
  else{
    laserCounter = -1;
    return false;
  }
}

//----------------------------------------------------------------Timer

long getSystemTime() {
	return millis();
}
void resetSystemTime() {
	// do nothing at this moment since haven't decided we need it or not
	return;
}

void wait(float sec) {
   // delay in seconds
	delay(sec*1000);
}



void setAttackMode(int a){
  attackMode=a;
}

int getAttackMode(){
  return attackMode;
}

void setRobotRole(int a){
  robotRole = a;
}

int getRobotRole(){
  return robotRole;
}

void setRobotMode(int a){
  robotMode = a;
}

int getRobotMode(){
  return robotMode;
}



int getTargetMode(){
  return targetMode;
}

void setTargetMode(int a){
  targetMode = a;
}

bool getKickoff(){
  return kickoff;
}

void setKickoff(bool a){
  kickoff = a;
}

void setBotNum(int a){
  botNum = a;
}

int getBotNum(){
  return botNum;
}


//----------------------------------------------------------------helper functions
