 /*********************************************************************/
// Maple Tech Lib 2017/8/15
// Version: v1.1
/*********************************************************************/
// Naming Rules:
// Varible
//     g_state
// Constant
//     BALL_THRESH
// Function:
//     getBall()
/*********************************************************************/


#ifndef FUNC_H
#define FUNC_H

#include <Arduino.h>
#include <Wire.h>
#include "compass.h"
#include "button.h"
#include "led.h"
#include "shooter.h"
#include <PID_v1.h>
#include "compoundEye.h"
#include "DFUltraTrig.h"

// #define TURNING_SPEED 7
#define MOVE_CORRECTION_RATIO 0.5

#define OFF 0
#define ON 1
#define BLINK 2

// #define ORANGE 1
#define YELLOW 1
#define BLUE 0

#define OFFENSE 1
#define DEFENSE 2
#define RONALDO 3

#define OFFENSE_SWITCH 1
#define DEFENSE_SWITCH 2
#define DEFAULT_SWITCH 3
#define OFFENSE_FIX 4
#define DEFENSE_FIX 5



#define JM 1
#define SMALL_ULTRA 1
#define INFRARED 2
#define CM 3
#define LARGE_ULTRA 4
#define BNO055 5
#define FRONT 0
#define RIGHT 1
#define BACK 2
#define LEFT 3

#define MAX_SPEED 90
#define STOP 360
//the representaion of stop in move
#define BLOCKED 361 
//the representaion of blocked in move


//#define H_SIZE 160 //old robot
#define H_SIZE 171 //new robot
#define V_SIZE 232//210

struct posInfo { int posX; int posY; bool isXBlocked; bool isYBlocked; long timestamp; };
struct posData { int posX; int posY; unsigned long timeX; unsigned long timeY; };
struct moveInfo { int dir; int dist; };
struct moveType { int speed; int direction; int target; };
static posData prevPosition;
static posInfo currPosition;



const int toleranceH = 100;
const int toleranceV = 120;//40
const int thresHoldh = H_SIZE-toleranceH; 
const int thresHoldv = V_SIZE-toleranceV; 



static const int FRONT_LIGHT_THRES=233;  
static const int BACK_IN_LIGHT_THRES=233;
static const int BACK_OUT_LIGHT_THRES=0;
static const int LEFT_IN_LIGHT_THRES=221;
static const int LEFT_OUT_LIGHT_THRES=211;
static const int RIGHT_IN_LIGHT_THRES=201;
static const int RIGHT_OUT_LIGHT_THRES=241;


static const int FRONT_IN_LIGHT_PIN=A0;
static const int BACK_IN_LIGHT_PIN=A3;
static const int LEFT_IN_LIGHT_PIN=A1;
static const int RIGHT_IN_LIGHT_PIN=A2;
static const int COMPASS_PIN=4;
static const int LEFT_DIS_PIN=0;
static const int RIGHT_DIS_PIN=1;
static const int FRONT_DIS_PIN=2;
static const int BACK_DIS_PIN=3;
// static const int EYE_PORT_PIN=13;
// static const int EYE_VALUE_PIN=14;
static const int LAZER_PIN = A7;
static const int SHOOT_PIN = A6;
static const int BUTTON_PIN[]= {10,11,12,30,31};
static const int DRIBBLER_PIN_1=A9;
static const int DRIBBLER_PIN_2=A7;
static const int DRIBBLER_PIN_3=A8;


static const int DRIB_SWITCH_PIN=29;

//I2C ADDRESSES
#define TCAADDR 0x70



//constant for motors

static int PWM[5] = {0,6,7,8,9};// pin number of motor 1,2,3,4's pwm
static int DIR[5] = {0,2,3,4,5};// pin number of motor 1,2,3,4's direction
static const int SERVO_PIN = 42;
static int motor_prob[5] = {0,1,1,1,1};







// -------------variables --------------
//global variable to control behaviour

#include "config.h"


//----------------------------------------------------------------setup functions
//setup functions
void setSerialMonitor();

void setMotorsDir(int dir1, int dir2, int dir3, int dir4);
void setupMotors();
void setEyeType(int type);
void setAngleThres(int angle);
int getAngleThres();
void setTurnSpeed(int s);
void setTurningMode(int mode);
int getTurningMode();
void setMotorMode(int mode);
int getMotorMode();
void setMotorChange(int change);
double getMotorChange();

//----------------------------------------------------------------sensors



//void getUltras(int ultras[]);





//void getAllGrey(int greys[]);


//----------------------------------------------------------------Kicker functions
//kicker related
void setKicker(int state);
bool getLaser();
bool hasBall();
//----------------------------------------------------------------Motion functions

int getTarget();
void setTarget(int newTarget);
int getSpeed();
void setSpeed(int newSpeed);
int getDir();
void setDir(int newDirection);


//----------------------------------------------------------------motors
void move();
int sinLaw(int degree1,int degree2,int speed2);
int getAngleDif(int target, int currentValue);
int getAbsAngle(int a);
void setMotor(int which,int motorSpeed);
void setMotors(int speed1, int speed2, int speed3, int speed4);
void setMotorsGradual(int speed1, int speed2, int speed3, int speed4);
void setupMotors();
double toRadian(int);
double toDegree(double);


void STOPNOW();


long getSystemTime();
void resetSystemTime();

void wait(float sec);   // delay in seconds

void setMoveCorrectionRatio(double);


void setAttackMode(int a);
int getAttackMode();

void setRobotRole(int a);
int getRobotRole();

void setRobotMode(int a);
int getRobotMode();
//helper functions
void setBotNum(int a);

int getBotNum();

int getTargetMode();

void setTargetMode(int a);


void setKickoff(bool a);
bool getKickoff();











#endif
