#include "button.h"
#include "player.h"

const int TOTAL=5;
const int TOTAL_STATE=2;

int buttonState[TOTAL]={0,0,0,0,0};
int buttonP[TOTAL] = {0,0,0,0,0};//record if button is pressed
int previousButtonPressed[TOTAL]={0,0,0,0,0};
bool longPressed[TOTAL]={0,0,0,0,0};

//the function to process all the information from buttons

void updateButton(){
  
  long now=millis();

  for(int i=1;i<=TOTAL;i++){
    buttonP[i]=!digitalRead(BUTTON_PIN[i]);
    if(buttonP[i]==1&&previousButtonPressed[i]==0){
      buttonState[i]=(buttonState[i]+1)%TOTAL_STATE;
    }
    previousButtonPressed[i] = buttonP[i];
  }

}

bool buttonPressed(int which){
  return !digitalRead(BUTTON_PIN[which])==1;
}

bool getButtonState(int which){
  return buttonState[which];
}

bool buttonLongPressed(int which){
  return longPressed[which];
}

void clearButtonState(){
  for(int i=0;i<TOTAL;i++){
    buttonState[i]=0;
  }
}

void setupButton(){
  for(int i = 1;i<=TOTAL;i++){
    pinMode(BUTTON_PIN[i],INPUT);
  }
}

void checkButton(){

  if (buttonPressed(3))
  {
    if (getAttackMode() == YELLOW)
    {
      setAttackMode(BLUE);
    }
    else{
      setAttackMode(YELLOW);
    }
  }

  if (getAttackMode() == YELLOW)
  {
    setLEDState(3,1);
  }
  else
  {
    setLEDState(3,0);
  }
  

  if (buttonPressed(2)) {
    setLEDState(2,1);
    calibrateThreshold();
  }
  else
  {
    setLEDState(2,0);
  }
}
