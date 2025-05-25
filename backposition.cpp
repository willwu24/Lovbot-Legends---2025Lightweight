#include "Backposition.h"

int AREA1 = 135;
int AREA2 = 180;
int AREA3 = 225;
int AREA4 = 90;
int AREA5 = STOP;
int AREA6 = 270;
int AREA7 = 45;
int AREA8 = 0;
int AREA9 = 315;

int backSpeed = 60;
int closeBoundary = 70;
int farBoundary = 90;
int backBoundary = 35;
int frontBoundary = 60;
int horizontalBlockRange = 80;

void backPosition() 
{
  setAngleThres(20);
  setTarget(0);
	int uLeft = getLeftDistanceCM();
	int uRight = getRightDistanceCM();
	int uFront = getFrontDistanceCM();
	int uBack = getBackDistanceCM();
  /*
  Serial.print("RAW: ");
  Serial.println(getRawLeftDistance());
  Serial.print("CM: ");
  Serial.println(uLeft);*/
  
	if(( uLeft + uRight > horizontalBlockRange ) && ( uFront + uBack > 100 )) 
	{
		// Non-block
	 	notBlocked();

	} 
	else 
	{
		//Block
		blocked();

	}
}

void blocked() 
{
  int uLeft = getLeftDistanceCM();
  int uRight = getRightDistanceCM();
  int uFront = getFrontDistanceCM();
  int uBack = getBackDistanceCM();
  int dir = STOP;
  if( uLeft + uRight > horizontalBlockRange){
    if(uLeft<uRight){//using left
      if( uLeft < closeBoundary) dir = AREA4;
      else if(uLeft <farBoundary) dir = AREA5;
      else dir = AREA6;
    }
    else{//using right
      if( uRight < closeBoundary) dir = AREA6;
      else if(uRight <farBoundary) dir = AREA5;
      else dir = AREA4;
    }
  }
  else if(uFront + uBack > 100 ){
    if(uBack > frontBoundary ) 
      {
        dir = AREA2;
      }
      //Area 8
      else if( uBack < backBoundary )
      {
        dir = AREA8;
      }
      //Area 5
      else 
      {
        dir = AREA5;      
      }
  }
  setDir(dir);
}

void notBlocked() 
{
	int uLeft = getLeftDistanceCM();
	int uRight = getRightDistanceCM();
	int uFront = getFrontDistanceCM();
	int uBack = getBackDistanceCM();
	int dir = STOP;

	// AREA 1,4,7
  if(uLeft<uRight){//using left
  	if( uLeft < closeBoundary) 
  	{
  		//Area 1
  		if(uBack > frontBoundary ) 
  		{
  			dir = AREA1;
  		}
  		//Area 7
  		else if( uBack < backBoundary )
  		{
  			dir = AREA7;
  		}
  		//Area 4
  		else 
  		{
  			dir = AREA4;			
  		}

  	}
  	// AREA 2,5,8
  	else if( uLeft < farBoundary )
  	{
      //Area 2
      if(uBack > frontBoundary ) 
      {
        dir = AREA2;
      }
      //Area 8
      else if( uBack < backBoundary )
      {
        dir = AREA8;
      }
      //Area 5
      else 
      {
        dir = AREA5;      
      }
  	}
  	// AREA 3,6,9
  	else 
  	{
      //Area 3
      if(uBack > frontBoundary ) 
      {
        dir = AREA3;
      }
      //Area 9
      else if( uBack < backBoundary )
      {
        dir = AREA9;
      }
      //Area 6
      else 
      {
        dir = AREA6;      
      }
  	}
  }
  //using right
  else{
    // AREA 3,6,9
    if( uRight < closeBoundary) 
    {
      //Area 3
      if(uBack > frontBoundary ) 
      {
        dir = AREA3;
      }
      //Area 9
      else if( uBack < backBoundary )
      {
        dir = AREA9;
      }
      //Area 6
      else 
      {
        dir = AREA6;      
      }
  
    } 
    // AREA 2,5,8
    else if( uRight < farBoundary )
    {
      //Area 2
      if(uBack > frontBoundary ) 
      {
        dir = AREA2;
      }
      //Area 8
      else if( uBack < backBoundary )
      {
        dir = AREA8;
      }
      //Area 5
      else 
      {
        dir = AREA5;      
      }
    }
    // AREA 1,4,6
    else 
    {
      //Area 1
      if(uBack > frontBoundary ) 
      {
        dir = AREA1;
      }
      //Area 6
      else if( uBack < backBoundary )
      {
        dir = AREA6;
      }
      //Area 4
      else 
      {
        dir = AREA4;      
      }
    }
  }

	setDir(dir);
	setSpeed(backSpeed);

}

void setPos(int p){
  //stub
  //TODO: finish the functionality of setPos
}
