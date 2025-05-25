#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "player.h"

void setupBluetooth();
void transmit(int);
int receive();
void transmitData();
void receiveData();
void sendBallStatus();
int getReceiveArr(int);
int getSendArr(int);
void changeSendArr(int,int);
int getArrLength();
#endif