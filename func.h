#ifndef FUNC_H
#define FUNC_H

struct posInfo { int posX; int posY; bool isXBlocked; bool isYBlocked; long timestamp; };
struct posData { int posX; int posY; unsigned long timeX; unsigned long timeY; };
struct moveInfo { int dir; int dist; };
struct moveType { int speed; int direction; int target; };
struct botZeroInfo {int motor_dir[4] = {0,-1,-1,-1}; int ballClosest = 750;};
struct botOneInfo {int motor_dir[4] = {0,-1,-1,-1}; int ballClosest = 750;};
struct botCurrInfo {int motor_dir[4]; int ballClosest;};
static posData prevPosition;
static posInfo currPosition;

#include <Arduino.h>
#include <Wire.h>
#include "compass.h"
#include "button.h"
#include "led.h"
#include "shooter.h"
#include <PID_v2.h>
#include "compoundEye.h"
#include "DFUltraTrig.h"
#include "config.h"

// -------------------------- Constants & Macros --------------------------

#define MOVE_CORRECTION_RATIO 0.25

#define OFF     0
#define ON      1
#define BLINK   2

#define YELLOW  1
#define BLUE    0

#define OFFENSE   1
#define DEFENSE   2

#define OFFENSE_SWITCH  1
#define DEFENSE_SWITCH  2
#define DEFAULT_SWITCH  3
#define OFFENSE_FIX     4
#define DEFENSE_FIX     5

#define JM            1
#define SMALL_ULTRA   1
#define INFRARED      2
#define CM            3
#define LARGE_ULTRA   4
#define BNO055        5

#define FRONT   0
#define RIGHT   1
#define BACK    2
#define LEFT    3

#define MAX_SPEED  99
#define STOP       360   // Representation of stop in move()
#define BLOCKED    361   // Representation of blocked in move()

#define H_SIZE 450  // Horizontal field size (new robot)
#define V_SIZE 550  // Vertical field size

const int toleranceH = 100;
const int toleranceV = 120;//40
const int thresHoldh = H_SIZE-toleranceH; 
const int thresHoldv = V_SIZE-toleranceV; 

// -------------------------- Pin Configuration --------------------------

static const int LAZER_PIN       = A6;
static const int SHOOT_PIN       = A12;
static const int BUTTON_PIN[]    = {10, 11, 12, 30, 31};
// static const int DRIBBLER_PIN_1  = A9;

#define TCAADDR 0x70  // I2C Multiplexer Address

// -------------------------- Motor Configuration --------------------------

static int PWM[5]       = {0, 6, 7, 8, 9};   // PWM pins for motors 1–4
static int DIR[5]       = {0, 2, 3, 4, 5};   // Direction pins for motors 1–4
static int motor_prob[5]= {0, 1, 1, 1, 1};   // Motor signal polarity correction

// -------------------------- Function Prototypes --------------------------

// Setup
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

// Motion
int getTarget();
void setTarget(int newTarget);
int getSpeed();
void setSpeed(int newSpeed);
int getDir();
void setDir(int newDirection);

void move();
int sinLaw(int degree1, int degree2, int speed2);
int getAngleDif(int target, int currentValue);
int getAbsAngle(int a);
double toRadian(int degree);
double toDegree(double radian);

void setMotor(int which, int motorSpeed);
void setMotors(int speed1, int speed2, int speed3, int speed4);
void setMotorsGradual(int speed1, int speed2, int speed3, int speed4);
void STOPNOW();

// Laser / Ball
void setKicker(int state);
bool getLaser();
bool hasBall();

// Time / Utility
long getSystemTime();
void resetSystemTime();
void wait(float sec);

void setMoveCorrectionRatio(double ratio);

// Robot State
void setAttackMode(int a);
int getAttackMode();

void setRobotRole(int a);
int getRobotRole();

void setRobotMode(int a);
int getRobotMode();

void setBotNum(int a);
int getBotNum();

int getTargetMode();
void setTargetMode(int a);

void setKickoff(bool a);
bool getKickoff();

#endif // FUNC_H
