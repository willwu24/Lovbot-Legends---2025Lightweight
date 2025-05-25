#include "core_pins.h"
#include "DFUltraTrig.h"

#define    VELOCITY_TEMP(temp)       ( ( 331.5 + 0.6 * (float)( temp ) ) * 100 / 1000000.0 ) // The ultrasonic velocity (cm/us) compensated by temperature

#define ULTRA_NUM 4

int trigechoPin[ULTRA_NUM] = {A9, A13, A8, A12};//A14 for front
uint16_t distance;
uint32_t pulseWidthUs;

long lastProcessTime = 0;

int ultraCount = 0;
//								Front,Right,Back,Left
int ultraReadings[ULTRA_NUM] = {0,0,0,0};



void processDFUltraTrig(){
	if(millis()-lastProcessTime>3){//3
      int16_t  dist, temp;
      pinMode(trigechoPin[ultraCount],OUTPUT);
      digitalWrite(trigechoPin[ultraCount],LOW);
    
      digitalWrite(trigechoPin[ultraCount],HIGH);//Set the trig pin High
      delayMicroseconds(10);     //Delay of 10 microseconds
      digitalWrite(trigechoPin[ultraCount],LOW); //Set the trig pin Low

      pinMode(trigechoPin[ultraCount],INPUT);//Set the pin to input mode                      BREAKS WHITE LINE
      pulseWidthUs = pulseIn(trigechoPin[ultraCount],HIGH,40000);//Detect the high level time on the echo pin, the output high level time represents the ultrasonic flight time (unit: us)
      //pulseWidthUs = pulseIn(trigechoPin[ultraCount],LOW,40000);
      distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;//The distance can be calculated according to the flight time of ultrasonic wave,/
                                                         //and the ultrasonic sound speed can be compensated according to the actual ambient temperature
      ultraReadings[ultraCount] = distance;
      // if(getBotNum()==0&&ultraCount==0){//TEMPORARY
      //   ultraReadings[0]= 1000;
      // }
      // Serial.print(ultraReadings[0]);
      // Serial.print("   ||  ");
      // Serial.print(ultraReadings[1]);
      // Serial.print("   ||  ");
      // Serial.print(ultraReadings[2]);
      // Serial.print("   ||  ");
      // Serial.println(ultraReadings[3]);
      lastProcessTime = millis();
      ultraCount=(ultraCount+1)%ULTRA_NUM;
   }
}
int getUltra(int which){
	return ultraReadings[which];
}

int getUltraFront(){
  return getUltra(0);
  //return getUltra(0);
}

int getUltraRight(){
  return getUltra(1);
}

int getUltraBack(){
  return getUltra(2);
}

int getUltraLeft(){
  return getUltra(3);
}

