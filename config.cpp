#include "config.h"

int motor_dir[5];

void setupVariables(){
  int n = sizeof(motor_dir) / sizeof(motor_dir[0]);
  if(getBotNum()==0){// my bot
    int new_motor_dir[5] = {0,-1,-1,-1,-1};
    for(int i = 0; i<n; i++){
      motor_dir[i] = new_motor_dir[i];
    }
  }
  else{//robert bot
    int new_motor_dir[5] = {0,1,-1,-1,-1};
    for(int i = 0; i<n; i++){
      motor_dir[i] = new_motor_dir[i];
    }
  }
}