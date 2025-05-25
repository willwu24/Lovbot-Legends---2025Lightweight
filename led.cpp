#include "led.h"
#include "player.h"

const int TOTAL_LED=5;
int LEDStates[TOTAL_LED] = {0,0,0,0,0};
int LEDPins[TOTAL_LED]={33,34,35,36,37};
long blinkTime[TOTAL_LED]={0,0,0,0,0};

void setLED(){
  for(int i=0;i<TOTAL_LED;i++){
    if(!LEDStates[i]) digitalWrite(LEDPins[i],LOW);
    else if(LEDStates[i]==ON) digitalWrite(LEDPins[i],HIGH);
    else if(LEDStates[i]==BLINK){
      long now = millis();
      
      if(now-abs(blinkTime[i])>300){
        if(blinkTime[i]>0){
          blinkTime[i]=-now;
          digitalWrite(LEDPins[i],LOW);
        }
        else{
          blinkTime[i]=now;
          digitalWrite(LEDPins[i],HIGH);

        }
      }
    }
  }
}

void setLEDState(int which, int state){
  //which-=1;
  if(which<TOTAL_LED&&which>=0&&state>=0&&state<3) LEDStates[which]=state;
}

void setLEDAll(int state){
  for(int i=0;i<TOTAL_LED;i++){
    LEDStates[i]=state;
    if(state==BLINK) blinkTime[i]=millis();
  }
  setLED();
}
void clearLEDState(){
  setLEDAll(OFF);
}

void checkLED(){

  if (getKickoff()){
    setLEDState(4,1);
  }
  else{
    setLEDState(4,0);
  }

  if (whiteDetected()){
    setLEDState(0,1);
  }
  else
  {
    setLEDState(0,0);
  }
}
