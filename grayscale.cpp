#include "Grayscale.h"
int readPins[] = {A0,A1};
int readPinsSize = 2;
int previousAngle = -1;
double magnitude;
double xDistance = 0;
double yDistance = 0;
// int previousSpeed;

int selectorPins[] = {38,39,40,41};
int selectorPinsSize = 4;
int whiteAngle;
int previousWhiteAngle;

const int sensorSize = 32;
int lastAngle;
bool flip = false;
bool firstFlip = true;
unsigned long lastWhiteTime = 0;
bool lastFlip = false;

int calibrationTimer = 0;

int sensorReadings[sensorSize];

int minSensorThres[sensorSize];
int maxSensorThres[sensorSize];
int sensorThres[sensorSize];
bool sensorWhite[sensorSize];


int cornerCounter = 0;
std::queue<int> pastWhites;
bool inCorner = false;
// const int pastStored = 5;
int sensorWhiteHistory[sensorSize];
int whiteCountHistory;





bool touchWhite = 0;
int whiteCount = 0;
bool firstWhite = 0;
int firstWhiteIndex = -1;
int firstWhiteAngle = -1;
unsigned long lastFirstWhite = -1;

const double sensitivity = 0.30;//lower means whiteline more detected?, CHANGE BACK TO 0.2

double whiteLineKp = 5.0, whiteLineKi = 0.0, whiteLineKd = 0.0;
double whiteLineSetPoint, whiteLineInput, whiteLineOutput;



PID whiteLinePID(&whiteLineInput, &whiteLineOutput, &whiteLineSetPoint, whiteLineKp, whiteLineKi, whiteLineKd, DIRECT);



void initSensors(){
  // for(int i = 0; i<pastStored; i++){
  //   pastStored.push(-1);

  // }
  for(int selectorPinsCount=0; selectorPinsCount<selectorPinsSize; selectorPinsCount++){
    pinMode(selectorPins[selectorPinsCount],OUTPUT);
  }
  for(int readPinCount=0; readPinCount<readPinsSize; readPinCount++){
    pinMode(readPins[readPinCount],INPUT);
  }
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    maxSensorThres[sensorCount] = 0;
    minSensorThres[sensorCount] = 999;
  }
  //calibrateThreshold();
  whiteLinePID.SetMode(AUTOMATIC);
  whiteLinePID.SetOutputLimits(-50, 50);
  whiteLineSetPoint = 0;
}

void calibrateThreshold(){
  // int tolerance = 50;//10
  Serial.println("calibrateThreshold");
    for(int sensorCount = 0; sensorCount<sensorSize/2; sensorCount++){
        digitalWrite(selectorPins[0],sensorCount&0b0001);
        digitalWrite(selectorPins[1],(sensorCount&0b0010)>>1);
        digitalWrite(selectorPins[2],(sensorCount&0b0100)>>2);
        digitalWrite(selectorPins[3],(sensorCount&0b1000)>>3);
        int reading = analogRead(readPins[0]);
        int reading16 = analogRead(readPins[1]);
        maxSensorThres[sensorCount] = max(maxSensorThres[sensorCount],reading);
        maxSensorThres[sensorCount+16] = max(maxSensorThres[sensorCount+16],reading16);
        minSensorThres[sensorCount] = min(minSensorThres[sensorCount],reading);
        minSensorThres[sensorCount+16] = min(minSensorThres[sensorCount+16],reading16);
        sensorThres[sensorCount] = minSensorThres[sensorCount]+(maxSensorThres[sensorCount]-minSensorThres[sensorCount]) * sensitivity;
        sensorThres[sensorCount+16] = minSensorThres[sensorCount+16]+(maxSensorThres[sensorCount+16]-minSensorThres[sensorCount+16]) * sensitivity;
    }
}

void calibrateLightweightThreshold(){
  if (buttonPressed(0) && buttonPressed(4) && !buttonPressed(1) && !buttonPressed(3) && !buttonPressed(2)) {
    calibrationTimer = millis();
    setLEDState(4,1);
  }
  else if(millis()-calibrationTimer < 1000){
    setLEDState(4,1);
    calibrateThreshold();
    setTarget(180);
    setDir(STOP);
  }
  else if(millis()-calibrationTimer < 2000){
    setLEDState(4,1);
    calibrateThreshold();
    setTarget(0);
    setDir(STOP);
  }
  else if(millis()-calibrationTimer < 5000){
    setLEDState(4,1);
    calibrateThreshold();
    setSpeed(15);
    setDir(0);
    setTarget(0);
    setAngleThres(10);
  }
  else if(millis()-calibrationTimer < 7000){
    setLEDState(4,1);
    calibrateThreshold();
    setSpeed(15);
    setDir(270);
    setTarget(0);
    setAngleThres(10);
  }
  else if(millis()-calibrationTimer < 10000){
    setLEDState(4,1);
    calibrateThreshold();
    setSpeed(15);
    setDir(190);
    setTarget(0);
    setAngleThres(10);
  }
  else if(millis()-calibrationTimer < 11000){
    setLEDState(4,1);
    calibrateThreshold();
    setSpeed(15);
    setDir(90);
    setTarget(0);
    setAngleThres(10);
  }
  else
  {
    setLEDState(4,1);
  }
}


void resetCalibrateThreshold(){
  for(int i=0; i<sensorSize; i++){
    sensorThres[i]=0;
  }
}

void processSensors(){

  int newTouchWhite = 0;
  whiteCount=0;
  whiteCountHistory=0;
  firstWhite = 0;
  for(int sensorCount = 0; sensorCount<sensorSize/2; sensorCount++){
    digitalWrite(selectorPins[0],sensorCount&0b0001);
    digitalWrite(selectorPins[1],(sensorCount&0b0010)>>1);
    digitalWrite(selectorPins[2],(sensorCount&0b0100)>>2);
    digitalWrite(selectorPins[3],(sensorCount&0b1000)>>3);
    sensorReadings[sensorCount]=analogRead(readPins[0]);
    sensorReadings[sensorCount+16]=analogRead(readPins[1]);

  }
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){ //go through each sensor
    sensorWhite[sensorCount] = sensorReadings[sensorCount]>sensorThres[sensorCount]; //show the state of the sensor in this array
    whiteCount+=sensorWhite[sensorCount];
    if(sensorReadings[sensorCount]>sensorThres[sensorCount]){
      sensorWhiteHistory[sensorCount]=1;
    }
    whiteCountHistory+=sensorWhiteHistory[sensorCount];
    newTouchWhite = newTouchWhite||sensorWhite[sensorCount];
    if(newTouchWhite&&!touchWhite){
      touchWhite=true;
      firstWhiteIndex=sensorWhite[sensorCount]?sensorCount:sensorCount+32;// what, 24 before
      firstWhite = true;
    }
  }
  touchWhite=newTouchWhite;
  //Serial.print(newTouchWhite);
  firstWhiteIndex=touchWhite?firstWhiteIndex:-1;
  firstWhiteAngle = touchWhite?((firstWhiteIndex*(360/32))-90)%360:-1;//maybe
  firstWhiteAngle = (firstWhiteAngle+360)%360;
  
  // if (whiteDetected())
  // {
  //   processWhiteAngle();
  // }

}

void printSensorsThres(){
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    Serial.print(sensorThres[sensorCount]);
    Serial.print(", ");
  }
  Serial.println();
}

void printSensorsReading(){
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    Serial.print(sensorReadings[sensorCount]);
    Serial.print(", ");
  }
  Serial.println();
}

void printSensorsWhite(){//print an array for white detection
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    Serial.print(sensorWhite[sensorCount]);
    Serial.print(", ");
  }
  Serial.println();
}
void printWhiteSensors(){//only print the sensor index if white is detected
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    if(sensorWhite[sensorCount]){
      Serial.print(sensorCount);
      Serial.print(", ");
    }
  }
  Serial.println();
}
void printMinThres(){
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    Serial.print(minSensorThres[sensorCount]);
    Serial.print(", ");
  
  }
  Serial.println();
}
void printMaxThres(){
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    Serial.print(maxSensorThres[sensorCount]);
    Serial.print(", ");
  }
  Serial.println();
}
void printDiffThres(){
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    Serial.print(maxSensorThres[sensorCount]-minSensorThres[sensorCount]);
    Serial.print(", ");
  }
  Serial.println();
}
int getFirstWhiteAngle(){
  return firstWhiteAngle;
}
bool getFirstWhite(){
  return firstWhite;
}
bool whiteDetected(){
  return touchWhite;
}



void printWhiteHistory(){
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    Serial.print(sensorWhiteHistory[sensorCount]);
    Serial.print(", ");
  }
  Serial.println();
}

int getWhiteCountHistory(){
  return whiteCountHistory;
}

void resetWhiteLine(){
  for(int sensorCount = 0; sensorCount<sensorSize; sensorCount++){
    sensorWhiteHistory[sensorCount]=0;
    whiteCountHistory=0;
  }
}

int getLightDif(int currentLight, int lastLight){//what
  int output = 0;
  int current = currentLight;
  int target = lastLight;

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

void processWhiteAngle(){
  double xClusterSum[16] = {0};
  double yClusterSum[16] = {0};
  double clusterAngle[16] = {0};
  double sensorAngle[sensorSize] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
  double length = 1;
  double xSum = 0;
  double ySum = 0;


  int clusterCount = 0;

  for (int i = 0; i < 32; i++)//what
  {
    if (sensorWhite[i])
    {
      xClusterSum[clusterCount] += sin(toRadian(360/sensorSize*i));
      yClusterSum[clusterCount] += cos(toRadian(360/sensorSize*i));
    }
    else
    {
      if (sensorWhite[i-1] && !sensorWhite[i])
      {
        clusterCount++;
      }
    }
  }

  //Count last cluster
  if (sensorWhite[31])
  {
    clusterCount++;
  }

  if (sensorWhite[0] && sensorWhite[31])//connect two ends?
  {
    xClusterSum[0] += xClusterSum[clusterCount-1];
    yClusterSum[0] += yClusterSum[clusterCount-1];

    xClusterSum[clusterCount-1] = 0;
    yClusterSum[clusterCount-1] = 0;
    clusterCount = clusterCount - 1;
  }

  // Serial.print(clusterCount);
  // Serial.print("       ");

  for (int j = 0; j < 16; j++)
  {
    double clusterLength = sqrt(pow(xClusterSum[j],2)+pow(yClusterSum[j],2));//make it regular length

    xClusterSum[j] = xClusterSum[j]/clusterLength;
    yClusterSum[j] = yClusterSum[j]/clusterLength;

    clusterAngle[j] = atan2(xClusterSum[j],yClusterSum[j]);
    clusterAngle[j] = clusterAngle[j] * (180/3.14);

  }

  for (int c = 0; c < clusterCount; c++)
  {
    xSum += xClusterSum[c];
    ySum += yClusterSum[c];
  }



  double rawAngle = atan2(xSum,ySum) * 180/3.14;

  rawAngle = (int(rawAngle) + 360)%360;

  // Serial.println(rawAngle);

  int finalAngle = rawAngle;
  if(!whiteDetected()){
    firstFlip=true;
  }
  else{

    if(firstFlip){
      firstFlip = false;
    }
    else if (whiteDetected() && abs(getLightDif(lastAngle, rawAngle)) > 120)
    {
      flip = !flip;
      lastWhiteTime = millis();
    }
  }
  if(!whiteDetected() && millis()-lastWhiteTime>3000){
    flip = 0;
  }

  // Serial.print(lastAngle);
  // Serial.print("  ");
  // Serial.print(rawAngle);
  // Serial.print("  ");
  // Serial.print(firstFlip);
  // Serial.print("  ");
  // Serial.print(flip);
  // Serial.print("  ");
  
  whiteAngle = ((finalAngle+180*flip)+270)%360;//270
  // Serial.print(whiteAngle);
  // Serial.print("       ");
  if (abs(xSum) < 0.01 && abs(ySum) < 0.01)
  {
    whiteAngle = previousWhiteAngle;
  }

  magnitude = (sqrt(pow(xSum,2) + pow(ySum,2)));
  if (flip)
  {
    magnitude = magnitude * -1;
  }
  //Serial.print(magnitude);



  previousWhiteAngle = whiteAngle;

  lastAngle = rawAngle;

  for (int j = 0; j < 16; j++)
  {
    // Serial.print(xClusterSum[j]);
    // Serial.print(" ");
    // Serial.print(yClusterSum[j]);
    // Serial.print("    ");
  }

  whiteLineInput = abs(xSum) + abs(ySum);
  whiteLinePID.Compute();
  xDistance = xSum;
  yDistance = ySum;
  // Serial.println(whiteLineInput);

  // printSensorsWhite();
  if(firstWhite){
    lastFirstWhite = millis();
    Serial.print(firstWhiteAngle);
    if((getFirstWhiteAngle()<60||getFirstWhiteAngle()>300)&&getUltraFront()<25&&(getUltraLeft()<30||getUltraRight()<30)&&(getEyeAngle()>270||getEyeAngle()<90)){
      cornerCounter++;
    }
    // else{
    //   cornerCounter = 0;
    //   inCorner = false;
    // }
  }
  if(millis()-lastFirstWhite>10000||getEyeAngle==-1||((getEyeAngle()>30&&getEyeAngle()<330)&&getUltraFront()>50)){//((getEyeAngle()>45&&getEyeAngle()<315)&&getUltraFront()>40)        //((getEyeAngle()<330&&getEyeAngle()>180&&getUltraRight()<40)||(getEyeAngle()>30&&getUltraLeft()<40)&&getUltraFront()>40)
    cornerCounter = 0;
  }
  if(cornerCounter>=2){
    inCorner = true;
  }
  else{
    inCorner = false;
  }
  //Serial.print(lastFirstWhite);

  // Serial.print("counter:");
  // Serial.print(cornerCounter);
}

int getWhiteAngle(){
  return whiteAngle;
}

int getRawWhiteAngle(){
  return (whiteAngle+180*flip)%360;
}

bool getFlip(){
  return flip;
}

double getMagnitude()
{
  return magnitude;
}

double getXSum(){
  return xDistance;
}

double getYSum(){
  return yDistance;
}

bool getInCorner(){
  return inCorner;
}

