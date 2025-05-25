#include "compoundEye.h"

int eyeNum = 18;
int eyePortThres[18] = {32,63,94,125,157,188,220,254,285,317,338,368,398,411,442,474,502,533}; //18 circular eyes 335-->338
// //                   0,20,40, 60, 80,100,120,140,160,180,200,220,240,260,280,300,320,340
//analog 
void eyeSetup(){
  pinMode(EYE_PORT_PIN, INPUT);
  pinMode(EYE_VALUE_PIN, INPUT);
}

//analog eye 
int getEyePort(){
  int portValue = analogRead(EYE_PORT_PIN);
  int port = -1;


  for(int i=0; i<eyeNum; i++){
    if(portValue<(eyePortThres[i]+eyePortThres[i+1])/2){
      port=i;
      break;
    }
    if(i==eyeNum-1){
      port=eyeNum-1;
    }
  }
  return port;
}

int getEyeValue(){
  int eyeValue = analogRead(EYE_VALUE_PIN);
  return eyeValue;
}
//both eye 

int getEyeAngle(){  
  int port = getEyePort();
  int degree = (port)*(360.0/(eyeNum));
  int frontAngle = analogRead(A3);
  if(frontAngle>60&&frontAngle<180){//ball is in the front, use it
    if(frontAngle<100){
      degree=10;
    }
    else if(frontAngle<130){
      degree=0;
    }
    else{
      degree=350;
    }
  }
  return degree;
}

int getEyeAngleSmooth(){
  int portValue = analogRead(EYE_PORT_PIN); // Read the port value
  int port = -1;
  float angle = 0.0;

  for(int i = 0; i < eyeNum; i++){
    // Determine which two thresholds the portValue falls between
    if(portValue < eyePortThres[i]){
      port = i;
      break;
    }
  }

  if(port == 0){ // If the value is below the first threshold
    // Interpolate between the first and the last threshold for a smooth transition
    float fraction = (float)(portValue - eyePortThres[eyeNum-1]) / (eyePortThres[0] - eyePortThres[eyeNum-1] + 360);
    angle = fraction * (360.0 / eyeNum);
  } else if(port > 0) {
    // Calculate the fraction of the portValue between the two surrounding thresholds
    float fraction = (float)(portValue - eyePortThres[port-1]) / (eyePortThres[port] - eyePortThres[port-1]);
    angle = (port-1 + fraction) * (360.0 / eyeNum);
  } else {
    // If the value is beyond the last threshold, wrap around to the beginning
    float fraction = (float)(portValue - eyePortThres[eyeNum-1]) / (360 - eyePortThres[eyeNum-1]);
    angle = (eyeNum-1 + fraction) * (360.0 / eyeNum);
  }

  return (int)angle;
}






