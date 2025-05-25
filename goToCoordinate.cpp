/*
                      GoToCoordinate.cpp

The point of this class is to find an efficient way to return to a certain position.

field is 180(vertical) cm by 180(horizontal) cm


*/


#include "GoToCoordinate.h"
#include "Led.h"
#include "jumpRemover.h"
#include "compass.h"
#include "Grayscale.h"
#include "Camera.h"
#include "func.h"

#define INVALID -10

#define NUM_OF_PARKING_SPOTS 1

#define FIND_PARKING_TIME 300

#define maxVALUE 1000

int backPositionSpeed = 30;
int maxDistance = 65; //how far does the robot start slowing down
int maxSpeedDiff = 27; //how much should the robot slow down by
int thresHold = 5;

//  -----  Coordinates of the Robot  -----
int xCO;
int yCO;


//  -----  Coordinates of the Destination  -----
int goToX = 0;
int goToY = 0;


//  is the robot parked?
bool parked = false;


//  -----  the parking spots (changeable)
//int parkingSpot [NUM_OF_PARKING_SPOTS][2] = {{90, 80}, {120, 50},{120,110},{60,110},{60,50}};
int parkingSpot [NUM_OF_PARKING_SPOTS][2] = {{70, 45}};







jumpRemover jRFront(5);
jumpRemover jRLeft(5);
jumpRemover jRBack(5);
jumpRemover jRRight(5);

int lastChange = -1;
int currentPos = 0;
const int origWaitTime = 500;
int waitTime = origWaitTime;

/*this variable is used to find if the robot is being blcoked by another object 
(if the length of the arena subtract minus the tolerance is larger 
than the the length the robot thinks the arena is, then there is something blocking the robot)
*/


//

void advancedParking(){

//  -----  find where the robot is  -----
   // goToX=parkingSpot[0][0];
   // goToY=parkingSpot[0][1];
  processCoordinate();
  //setGoToClosest();
  setGoToOrder();

  // Serial.print(goToX);
  // Serial.print(" ");
  // Serial.print(goToY);
  //   Serial.print(" ");
  //processCoordinate();
  

//  -----  Go To the Coordinate  -----

  goToCO();
}
int confirmLockTime = -1;

void goToCO(){
//  -----  What direction we want the robot to go in  -----
  int botAngle;

//  -----  how far the robot has to go  -----
  int v = goToY-yCO;
  int h = goToX-xCO;
  // Serial.print(xCO);
  // Serial.print(" ");
  // Serial.println(yCO);
//  -----  if the robot is in it's destination, make it stop until it sees the ball again

  if(parked){
    botAngle=STOP;
    setDir(botAngle);
    //Serial.print("Stopped");
    confirmLockTime = -1;
    if(whiteDetected()){
      setParked(false);
    }
  }
  else{
  //  -----  if the robot is within the threshold of it's destination, set Parked to true
    if(abs(v)<thresHold&&abs(h)<thresHold&&(getCompass()<getAngleThres()||getCompass>360-getAngleThres())&&!whiteDetected()){
      if(confirmLockTime ==-1){
        confirmLockTime=millis();
      }
      else if(millis()-confirmLockTime>200){
        setParked(true);
      }
    }
    else{
      confirmLockTime = -1;
      setParked(false);
      /*  -----  If the robot is blocked horizontly and vertically,  
             OR reached it's horizontal destination but is vertically blocked, 
             OR reached it's vertical destination but is horizontally blocked,
             THEN temprary stop moving.  -----

      */
     if((xCO == INVALID && yCO == INVALID)||(yCO == INVALID &&abs(h)<thresHold)  ||  (abs(v)<thresHold&& xCO == INVALID)){
        botAngle=STOP;
        // if(getHomeDistance()!=-1&&getHomeDistance<90){
        //   //Serial.print("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
        //   botAngle=0;
        // }
     }

      //  -----  if the robot reached its horizontal destination or if its horizontaly blocked, move vertically only  ----- 
      /*----------------------------

      We use the formula min(speed, (speed*abs(v))/maxDistance)
      to make the robot slow to a stop. When abs(v)>maxDistance,[]
      abs(v)/maxDistance>1, making the speed 'speed' (due to the min)
      so the slowing down only starts when the distance between the robot
      and it's destination is smaller than maxDistance

      -----------------------------*/
      else if(xCO == INVALID||abs(h)<thresHold){
        // if(whiteDetected()){
        //   int angleYellow = getYellowAngle();
        //   int distYellow = getYellowDistance();
        //   int angleBlue = getBlueAngle();
        //   int distBlue = getBlueDistance();
        //   int a = -1;
        //   if(angleYellow!=-1&&distYellow>distBlue){
        //     a=angleYellow;
        //   }
        //   else if(angleBlue!=-1&&distBlue>distYellow){
        //     a=angleBlue;
        //   }
        //   if(a!=-1){
        //     if(0<a&&a<180){
        //       botAngle=90;
        //     }
        //     else{
        //       botAngle = 270;
        //     }
        //   }
        // }
        // else{
      //  -----  if the distance is positive, the robot moves 0 degrees (which is up)
          if(v>0){
            botAngle=0;
            setExponentialSpeed(abs(v));
          }

      //  -----  if the distance is positive, the robot moves 180 degrees (which is down)
          else if(v<0){
            botAngle=180;
            setExponentialSpeed(abs(v));

          //}
        }
      } 


      //  -----  if the robot reached its vertical  destination or if its vertically blocked, move horizontaly only  -----

      else if(yCO == INVALID||abs(v)<thresHold){

          if(h>0){
            botAngle=90;
            setExponentialSpeed(abs(h));
          }

          else if(h<0){
            botAngle=270;
            setExponentialSpeed(abs(h));
          }

      } 
      //  -----  LAST CASE: both x and y needs moving  -----
      else{
      /*
                  (goToX,goToY)
                 /|
                / |
               /  |
              /   |
             /    |  v
            /     |
           /      |
          /_______|
       (xCO,yCO)   
              h
      */

      botAngle = atan2(h,v)*180/3.14159265;
      if(botAngle<0){
        botAngle+=360;
      }

      double distance=sqrt(h*h+v*v);
      setExponentialSpeed(distance);
    }
    // if(botAngle==STOP){
    //          Serial.print("Stopped");
    // }
    setDir(botAngle);

      //  -----  Prints for Debugging  -----

      // Serial.print(" Angle of direction: ");
      // Serial.print(botAngle);
      // Serial.print(" v: ");
      // Serial.print(v);
      // Serial.print(" h: ");
      // Serial.print(h);

      // Serial.print(" speed: ");
      // Serial.print((speed*distance)/maxDistance);
    }
  }
}
void setExponentialSpeed(int di){

  double dist = (di-thresHold)/(maxDistance-thresHold-0.0);

  int a = backPositionSpeed-maxSpeedDiff+maxSpeedDiff*sqrt(dist);

  //setSpeed(min(backPositionSpeed,backPositionSpeed-maxSpeedDiff+maxSpeedDiff*sqrt(dist));

  setSpeed(constrain(a,backPositionSpeed-maxSpeedDiff,backPositionSpeed));
}




void processCoordinate(){
  int left = getDistance(LEFT);
  int right = getDistance(RIGHT);
  int back = getDistance(BACK);
  int front = getDistance(FRONT);


  //Purify position
  jRFront.add(front);
  jRLeft.add(left);
  jRBack.add(back);
  jRRight.add(right);

  front=jRFront.getMedian();
  left=jRLeft.getMedian();
  back=jRBack.getMedian();
  right=jRRight.getMedian();


//  ----- check if the robot is horiontally blocked  -----
  if(left+right<thresHoldh){
    xCO=INVALID;
  }


  else{
    if(left<right){
      xCO=left;
    }
    else{
      xCO=H_SIZE-right;
    }
  }

 
  if(front+back<thresHoldv){
    yCO=INVALID;
    // if(getHomeDistance()==-1&&getOpposingDistance()>-1){
    //   if(getOpposingDistance()<90){
    //     yCO=V_SIZE-front;
    //   }
    //   //Serial.print("AAAAAAAAAAAAAAAAAAAAAA");
    //   if(getOpposingDistance()>130){
    //     yCO=back;
    //   }
    // }
  }


  else{
    if(back<front){
      yCO=back;
    }
    else{
      yCO=V_SIZE-front;
    }
  }
  if(xCO==INVALID&&yCO==INVALID){
    //yCO=back;
    //Serial.print("bothBlocked");
  }



//  -----  Prints for Debugging  -----

  // Serial.print("Destination: (");
  // Serial.print(goToX);
  // Serial.print(",");
  // Serial.print(goToY);
  // Serial.print(")  ");

  // Serial.print("Current Position: (");
  // Serial.print(xCO);
  // Serial.print(",");
  // Serial.print(yCO);
  // Serial.print(")    ");

  // Serial.print(" Front: ");
  // Serial.print(front);
  // Serial.print(" Left: ");
  // Serial.print(left);
  // Serial.print(" Back: ");
  // Serial.print(back);
  // Serial.print(" Right: ");
  // Serial.println(right);

  // Serial.print("      FrontPurified: ");
  // Serial.print(jRFront.getMedian());
  // Serial.print(" LeftPurified: ");
  // Serial.print(jRLeft.getMedian());
  // Serial.print(" BackPurified: ");
  
  // Serial.print(jRBack.getMedian());
  // Serial.print(" RightPurified: ");
  // Serial.print(jRRight.getMedian());

  // Serial.print("  {");
  // for(int i=0; i<5; i++){
  //   Serial.print(jRFront.getValue(i));
  //   Serial.print(", ");
  // }
  // Serial.print("}  ");

  // Serial.print("  {");
  // for(int i=0; i<5; i++){
  //   Serial.print(jRFront.getValueSorted(i));
  //   Serial.print(", ");
  // }
  // Serial.println("}  ");

  // Serial.print("    Hor: ");
  // Serial.print(left+right);
  // Serial.print(" Vert: ");
  // Serial.println(front+back);
}






void setGoToClosest(){

//  -----  find closest parkingSpot[] to go to  -----  

  int closest;
  int closestValue=maxVALUE; //  maxVALUE is 1000

//  -----  if the robot is horizontally blocked, find the closest vertical position  -----
  if(xCO==INVALID){ 


//  -----  go through each of the parkingSpots  -----  

    for(int i = 0;i < NUM_OF_PARKING_SPOTS; i++){

//  -----  compare the magnitude of the distance to the suspected mininum  -----

      int distSpot = abs(parkingSpot[i][1]-yCO);

//  -----  if the distance is smaller, set min to the distance

      if(distSpot<closestValue){
        closest=i;
        closestValue=distSpot;
      }
    }

  }


  else if(yCO==INVALID){ //find the closest x parking spot if y is blocked
    for(int i = 0;i < NUM_OF_PARKING_SPOTS; i++){
      int distSpot = abs(parkingSpot[i][0]-xCO);
      if(distSpot<closestValue){
        closest=i;
        closestValue=distSpot;
      }
    }
  }


//  -----  if both are not blocked, used the hypotenuse of the x and y to compare. 
  else{
    for(int i=0; i<NUM_OF_PARKING_SPOTS; i++){
      int distSpot = sqrt( pow(parkingSpot[i][0]-xCO,2)+pow(parkingSpot[i][1]-yCO,2));
      if(distSpot<closestValue){
        closest=i;
        closestValue=distSpot;
      }
    }
  }

  goToX=parkingSpot[closest][0];
  goToY=parkingSpot[closest][1];

}

void setGoToOrder(){
  if(parked||getDir()==STOP){
    if(lastChange==-1){
      lastChange=millis();
    }
    if(millis()-lastChange>waitTime){
      waitTime=origWaitTime;
      lastChange=-1;
      currentPos=(currentPos+1)%NUM_OF_PARKING_SPOTS;
      setParked(false);
    }
    waitTime=max(waitTime,0);
  }
  else{
    if(lastChange!=-1){
      waitTime-=(millis()-lastChange);
      lastChange=-1;
    }
  }
  goToX=parkingSpot[currentPos][0];
  goToY=parkingSpot[currentPos][1];

  // Serial.print(" millis: ");
  // Serial.print(millis());
  // Serial.print(" lastChange: ");
  // Serial.print(lastChange);
  // Serial.print(" botSpeed: ");
  // Serial.print(getSpeed());
  // Serial.print(" waitTime: ");
  // Serial.print(waitTime);
  // Serial.print(" timeLeft: ");
  // Serial.println(waitTime-(millis()-lastChange));
}


void processCameraPosition(){

}

int getCameraPositionX(){

}

int getCameraPositionY(){

}





void setGoToCO(int x, int y){
  goToX = x;
  goToY = y;
}

int getXCO(){
  return xCO;
}

int getYCO(){
  return yCO;
}
void setXCO(int xx){
  xCO = xx;
}

void setYCO(int yy){
  yCO = yy;
}
void setParked(bool yes){
  parked=yes;
  // setLEDState(0, yes);
}