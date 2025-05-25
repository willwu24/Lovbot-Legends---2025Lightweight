// #include "flyeye.h"


// #define JM 1


// int eye_type=0;

// int eyePortThres[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};// robert's work not mine 

// int maxEyeValue[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // easier to count 

// int thresEyeValue[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // largest value when there's no ball

// int getEyeValue(){
//   //IF needed:  write custom eye Value function
//   return getRawEyeValue();
// }

// int getEyePort(){
//   //IF needed: write custom eye port function
//   return getRawEyePort(); 
// }

// int getRawEyePort() {
//   if(eye_type=JM){
//     int value0 = JMEyeValue(0);
//     int value1 = JMEyeValue(1);
//     return value0>value1?8-JMEyePort(0):7+JMEyePort(1);
//   }
//   int* thres = eyePortThres;
//   int portValue = analogRead(EYE_PORT_PIN);
//   if(portValue<(thres[0]+thres[1])/2) return 1;
//   else if(portValue<(thres[1]+thres[2])/2) return 2;
//   else if(portValue<(thres[2]+thres[3])/2) return 3;
//   else if(portValue<(thres[3]+thres[4])/2) return 4;
//   else if(portValue<(thres[4]+thres[5])/2) return 5;
//   else if(portValue<(thres[5]+thres[6])/2) return 6;
//   else if(portValue<(thres[6]+thres[7])/2) return 7;
//   else if(portValue<(thres[7]+thres[8])/2) return 8;
//   else if(portValue<(thres[8]+thres[9])/2) return 9;
//   else if(portValue<(thres[9]+thres[10])/2) return 10;
//   else if(portValue<(thres[10]+thres[11])/2) return 11;
//   else if(portValue<(thres[11]+thres[12])/2) return 12;
//   else if(portValue<(thres[12]+thres[13])/2) return 13;
//   else return 14;
// }
// int getRawEyeValue(){
//   if(eye_type=JM){
//     int value0 = JMEyeValue(0);
//     int value1 = JMEyeValue(1);
//     return value0>value1?value0:value1;
//   }
//   return analogRead(EYE_VALUE_PIN);
// }


// void setEyeType(int type){
//   eye_type = type;
//   if(eye_type=JM){
//     initializeJMEye(0);
//     initializeJMEye(1);
//   }
// }


// void tcaselect(uint8_t i) {
//   if (i > 7) return;
//   Wire.beginTransmission(TCAADDR);
//   Wire.write(1 << i);
//   Wire.endTransmission();
// }

// void initializeJMEye(int which)
// {
//   tcaselect(which);
//   int i;
//   Wire.begin();
//   Wire.beginTransmission(0x01);
//   Wire.write(0x0e);
//   Wire.endTransmission();
//   while(Wire.available() > 0)
//     i = Wire.read();
//     Serial.println("reading");
//     Serial.println(i);
// }

// int JMEyePort(int which){
//   tcaselect(which);
//   Wire.begin();
//   Wire.beginTransmission(0x01);//address for compound eye
//   Wire.write(0x08);
//   Wire.endTransmission();
//   //Serial.print("port: ");
//   Wire.requestFrom(0x01,1);
//   while(Wire.available()){
//     int i = Wire.read();
//     return i;
//   }
// }

// int JMEyeValue(int which){
//   tcaselect(which);
//   Wire.begin();
//   Wire.beginTransmission(0x01);
//   Wire.write(0x09);
//   Wire.endTransmission();
//   //Serial.print("value:");
//   Wire.requestFrom(0x01,1);
//   while(Wire.available()){
//     int i = Wire.read();
//     return i;
//   }
// }


// void getMaxEyeValue(){ // print function-largest value when ball is detected 
//   int currentV = getEyeValue(); 
//   int currentP = getEyePort();
//    Serial.println(currentV);
//    Serial.println(currentP);
//   if(currentV>maxEyeValue[currentP]){  // if current value > the previous maxEyeValue then it will replace it with the larger number 
//     maxEyeValue[currentP] = currentV; 
//   }

//     Serial.print("maxEyeValue1 = ");
//     Serial.println(maxEyeValue[1]);
//     Serial.print("maxEyeValue2 = ");
//     Serial.println(maxEyeValue[2]);
//     Serial.print("maxEyeValue3 = ");
//     Serial.println(maxEyeValue[3]);
//     Serial.print("maxEyeValue4 = ");
//     Serial.println(maxEyeValue[4]);
//     Serial.print("maxEyeValue5 = ");
//     Serial.println(maxEyeValue[5]);
//     Serial.print("maxEyeValue6 = ");
//     Serial.println(maxEyeValue[6]);
//     Serial.print("maxEyeValue7 = ");
//     Serial.println(maxEyeValue[7]);
//     Serial.print("maxEyeValue8 = ");
//     Serial.println(maxEyeValue[8]);
//     Serial.print("maxEyeValue9 = ");
//     Serial.println(maxEyeValue[9]);
//     Serial.print("maxEyeValue10 = ");
//     Serial.println(maxEyeValue[10]);
//     Serial.print("maxEyeValue11 = ");
//     Serial.println(maxEyeValue[11]);
//     Serial.print("maxEyeValue12 = ");
//     Serial.println(maxEyeValue[12]);
//     Serial.print("maxEyeValue13 = ");
//     Serial.println(maxEyeValue[13]);
//     Serial.print("maxEyeValue14 = ");
//     Serial.println(maxEyeValue[14]);
// }


// void getThreshold(){//print function-largest value when no ball is detected
//   int currentV = getEyeValue(); 
//   int currentP = getEyePort(); 

//   if(currentV>thresEyeValue[currentP]){  // if current value > the previous the thresEyeValue it will replace it with the bigger number
//     thresEyeValue[currentP] = currentV;
//     if(currentV<0){// sometimes the value will be negative 
//        currentV = 0; 
//     }
//   }  
//    Serial.print("threEyeValue1 = ");
//    Serial.println(thresEyeValue[1]);  
//    Serial.print("threEyeValue2 = ");
//    Serial.println(thresEyeValue[2]);  
//    Serial.print("threEyeValue3 = ");
//    Serial.println(thresEyeValue[3]);  
//    Serial.print("threEyeValue4 = ");
//    Serial.println(thresEyeValue[4]);  
//    Serial.print("threEyeValue5 = ");
//    Serial.println(thresEyeValue[5]);  
//    Serial.print("threEyeValue6 = ");
//    Serial.println(thresEyeValue[6]);  
//    Serial.print("threEyeValue7 = ");
//    Serial.println(thresEyeValue[7]);  
//    Serial.print("threEyeValue8 = ");
//    Serial.println(thresEyeValue[8]);  
//    Serial.print("threEyeValue9 = ");
//    Serial.println(thresEyeValue[9]);  
//    Serial.print("threEyeValue10 = ");
//    Serial.println(thresEyeValue[10]);  
//    Serial.print("threEyeValue11 = ");
//    Serial.println(thresEyeValue[11]);  
//    Serial.print("threEyeValue12 = ");
//    Serial.println(thresEyeValue[12]);  
//    Serial.print("threEyeValue13 = ");
//    Serial.println(thresEyeValue[13]);  
//    Serial.print("threEyeValue14 = ");
//    Serial.println(thresEyeValue[14]);  
   
// }



// /* TODO:
//  * void getMaxEyeValues(): get the max eye value for every single port!!!
//  * void getThreshold(): get the largest eye value when no ball is detected!!!
//  * 
//  */
