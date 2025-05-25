// #include "followball.h"

// void followBall2023(){
//   int maxOffset = 90;
//   float offset = maxOffset * ((100 - float(getBallDistance()))/30);

//   float diff = (180 - abs(float(getBallAngle())-180))/180;
//   offset = offset*diff;

//   offset = constrain(offset, 0, 90);

//   int ballAngle = getBallAngle();
//   if (ballAngle == -1)
//   {
//     setDir(STOP);
//   }
//   else if (ballAngle > 345 || ballAngle < 15)
//   {
//     setDir(0);
//   }
//   else if(ballAngle < 180)
//   {
//     setDir(ballAngle+offset);
//     Serial.println(ballAngle + offset);
//   }
//   else
//   {
//     setDir(ballAngle-offset);
//     Serial.println(ballAngle - offset);
//   }
// }

// void followBallNew(){
//   setAngleThres(120);
  
//   int minBallRange=90;
//   int maxBallRange=125;
//   double minShift = 5;
//   double maxShift = 60;
//   double finalShift;
//   int ballRange = getBallDistance();
//   int ballDirection=getBallAngle();
//   int d;
//   finalShift = (ballRange-minBallRange)*(maxShift)/(maxBallRange-minBallRange)+minShift;
//   //Serial.println(finalShift);
//   if(ballDirection<=180){
//     finalShift=finalShift*constrain(ballDirection,0,90)/90;
//     d=(int)(ballDirection+finalShift)%360;
//   }
//   else{
//     finalShift=finalShift*constrain(360-ballDirection,0,90)/90;
//     d=(int)(ballDirection-finalShift)%360;
//   }
//   if((getBallAngle()+getCompass())%360<180)
//   {
//     setTarget(constrain((getCompass()+getBallAngle())%360,0,90));
//   }
//   else
//   {
//     setTarget(constrain((getCompass()+getBallAngle())%360,270,359));
//   }

//   setDir(d);
//   setSpeed(80);
// }


// void followBall(){
//   int minBallRange=90;
//   int maxBallRange=125;
//   double minShift = 5;
//   double maxShift = 60;
//   double finalShift;
//   int ballRange = getBallDistance();
//   int ballDirection=getBallAngle();
//   int d;
//   finalShift = (ballRange-minBallRange)*(maxShift)/(maxBallRange-minBallRange)+minShift;
//   //Serial.println(finalShift);
//   if(ballDirection<=180){
//     finalShift=finalShift*constrain(ballDirection,0,90)/90;
//     d=(int)(ballDirection+finalShift)%360;
//   }
//   else{
//     finalShift=finalShift*constrain(360-ballDirection,0,90)/90;
//     d=(int)(ballDirection-finalShift)%360;
//   }
//   /*Serial.print("ballDir:");
//   Serial.println(ballDirection);
//   Serial.print("finalShift:");
//   Serial.println(finalShift);
//   Serial.print("range:");
//   Serial.println(ballRange);*/
  

//   setDir(d);
//   setSpeed(80);
// }
