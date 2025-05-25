#include "compass.h"

long compass_max_value = 480;
int compass_type=0;
Adafruit_BNO055 bno = Adafruit_BNO055(55);
int heading = 359;
int lastRawValue = 0;

void bno_write(uint8_t i2c_addr, uint8_t reg, uint8_t data)  // write one BNO register
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);  // send stop
}

int getCompassDif(int target, int currentValue){
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


int getCompassRawValue(){ // original, not raw value 
  int rawValue = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();

  if (abs(getCompassDif(lastRawValue, rawValue)) > 100){
    heading = rawValue;
  }
  lastRawValue = rawValue;

  return rawValue;
}

int getCompass(){
  int rawDegree = getCompassRawValue();
  if(buttonPressed(1)){
    heading = rawDegree;
  }
  int calculated = (int) (rawDegree - heading + 360)%360;
  return calculated;
}




void setupCompass(){
  Serial.println("setting compass type to BNO055");
  if(!bno.begin()){
    Serial.println("Something is wrong with BNO055 connection");
    while(1);
  }
  bno.setExtCrystalUse(true);
  sensor_t sensor;
  bno.getSensor(&sensor);

  bno.setMode(0x0C);//NDOF_MODE

  bno_write(BNO_ADDR, PAGE_ID, 1);          // register page 1
  bno_write(BNO_ADDR, ACC_CONFIG, 0x0D);    // accel 2g range (default value 0x0D)
  bno_write(BNO_ADDR, PAGE_ID, 0);          // register page 0

  bno_write(BNO_ADDR, OPR_MODE, 0x0C);  // operating mode
  
  delay(10);                                // allow time for BNO to switch modes
}
