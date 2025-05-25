#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "player.h"

// === Initialization ===
void setupBluetooth();

// === Data Transmission ===
void transmit(int data);
void transmitData();

// === Data Reception ===
int receive();
void receiveData();

// === Send/Receive Array Access ===
int getReceiveArr(int index);
int getSendArr(int index);
void changeSendArr(int index, int data);
int getArrLength();

#endif // BLUETOOTH_H
