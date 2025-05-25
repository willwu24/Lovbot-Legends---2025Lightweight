#include "Distance.h"



//0x70 = front, 0x71 = right, 0x72 = back, 0x73 = left
byte distanceAddresses[] = {0x70,0x71,0x72,0x73};


uint16_t delay_time=0;
void setupDistance() {
  Serial.println("Setting up distance...");
  Wire.begin(); 
  for(int i = 0;i<NUM_OF_DISTANCE;i++){
    uint16_t measure_time=measure_time_50ms;
    change_mode(distanceAddresses[i],DISTANCEMODE_LONG,measure_time);
  }
  Serial.println("Distance setup complete!");
}
int getDistance(byte which){
  takeRangeReading(distanceAddresses[which]); //Tell the sensor to perform a ranging cycle
  word range = requestRange(distanceAddresses[which]); //Get the range from the sensor
  return range;
}
//Commands the sensor to take a range reading
void takeRangeReading(byte addr){
  Wire.beginTransmission(addr); //Start addressing
  Wire.write(RangeCommand); //send range command
  Wire.endTransmission(); //Stop and do something else now
}
//Returns the last range that the sensor determined in its last ranging cycle in centimeters. Returns 0 if there is no communication.
word requestRange(byte addr){
  Wire.requestFrom(addr, byte(2));
  if(Wire.available() >= 2){ //Sensor responded with the two bytes
    byte HighByte = Wire.read(); //Read the high byte back
    byte LowByte = Wire.read(); //Read the low byte back
    word range = word(HighByte,LowByte);
    //word range = makeWord(HighByte, LowByte); //Make a 16-bit word out of the two bytes for the range
    return range;
  }
  else {
    return word(0,0);
    //return word(0); //Else nothing was received, return 0
  }
}
/* Commands a sensor at oldAddress to change its address to newAddress
oldAddress must be the 7-bit form of the address that is used by Wire
7BitHuh determines whether newAddress is given as the new 7 bit version or the 8 bit version of the address
If true, if is the 7 bit version, if false, it is the 8 bit version
*/
void changeAddress(byte oldAddress, byte newAddress, boolean SevenBitHuh){
  Wire.begin();
  Wire.beginTransmission(oldAddress); //Begin addressing
  Wire.write(ChangeAddressCommand1); //Send first change address command
  Wire.write(ChangeAddressCommand2); //Send second change address command
  byte temp;
  if(SevenBitHuh){ temp = newAddress << 1; } //The new address must be written to the sensor
  else { temp = newAddress; } //in the 8bit form, so this handles automatic shifting
  Wire.write(temp); //Send the new address to change to
  Wire.endTransmission();
  Serial.println("finished changing address");
}
void change_mode(byte Address, byte mode, byte  time){
  Wire.beginTransmission(Address); //Begin addressing
  Wire.write(ChangModeCommand1); //Send first  command
  Wire.write(ChangModeCommand2); //Send second  command
  byte temp;
  temp=(mode<<4)|time;
  Wire.write(temp); //Send the new address to change to
  Wire.endTransmission();
}

void printDistance(){
  int frontDistance = getDistance(0);
  int rightDistance = getDistance(1);
  int backDistance = getDistance(2);
  int leftDistance = getDistance(3);
  Serial.print(" front: ");
  Serial.print(frontDistance); 

  //1
  Serial.print(" right: ");
  Serial.print(rightDistance); 

  //2
  Serial.print(" back: ");
  Serial.print(backDistance); 

  //3
  Serial.print(" left: ");
  Serial.println(leftDistance); 

  // Serial.print("vertical size");
  // Serial.println(frontDistance + backDistance);

  // Serial.print("horizontal size");
  // Serial.println(rightDistance + leftDistance);
}

int getFrontDistanceCM()
{
  return getDistance(0);
}

int getRightDistanceCM()
{
  return getDistance(1);
}

int getBackDistanceCM()
{
  return getDistance(2);
}

int getLeftDistanceCM()
{
  return getDistance(3);
}

//analog ultrasonic 
// #define TOTAL_ULTRASONIC 4
// int ultrasonicPin [TOTAL_ULTRASONIC] = {A1, A2, A3, A4};
// //dist = m*x + b;
// double m = 0.109686;
// double b = -8.45282;

// //returns distance in cm
// int getDistance(int side){
//     return m*analogRead(ultrasonicPin[side]);
// }

// void setupDistance(){
//   for(int ID = 0; ID < TOTAL_ULTRASONIC; ID++){
//     pinMode(ultrasonicPin[ID], INPUT);
//   }
// }