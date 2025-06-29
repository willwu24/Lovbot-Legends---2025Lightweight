  #include "bluetooth.h"

// === Constants and Globals ===
const int ARR_LENGTH = 4;
int lastCheckTime = 0;
int lastTransmitTime = 0;
int lastCheck = -2;
long bluetoothTime = -10000;

int sendArr[ARR_LENGTH];
int receiveArr[ARR_LENGTH];

// === Setup ===
void setupBluetooth() {
  Serial.print("Setting up bluetooth... ");
  for (int i = 0; i < ARR_LENGTH; i++) {
    receiveArr[i] = -1;
    sendArr[i] = -1;
  }
  Serial1.begin(9600);
  Serial.println("Success!");
}

// === Transmit ===
void transmitData() {
  Serial1.write(0x98); // Header
  int sum = 0;

  for (int i = 0; i < ARR_LENGTH; i++) {
    sum += sendArr[i];
    Serial1.write(sendArr[i]);
  }

  int checksum = sum % 256;
  Serial1.write(checksum);
  lastTransmitTime = millis();

  // Reset send array after transmission
  for (int i = 0; i < ARR_LENGTH; i++) {
    sendArr[i] = -1;
  }
}

void transmit(int data) {
  if (millis() - lastTransmitTime > 100) {
    Serial1.write(data);
    lastTransmitTime = millis();
  }
}

// === Receive ===
void receiveData() {
  int tempData[ARR_LENGTH];

  if (Serial1.available() >= ARR_LENGTH + 2) {
    byte header = Serial1.read();
    
    if (header == 0x98) {
      byte checksum = 0;

      for (int i = 0; i < ARR_LENGTH; i++) {
        tempData[i] = Serial1.read();
        checksum += tempData[i];
      }

      byte checksum_received = Serial1.read();
      if (checksum_received == (checksum % 256)) {
        for (int i = 0; i < ARR_LENGTH; i++) {
          receiveArr[i] = tempData[i];
        }
        bluetoothTime = millis();
      } else {
        Serial.print(" Bluetooth checksum error! ");
      }
    }
  } else {
    // Timeout logic: clear data if no update within 15ms
    if (millis() - bluetoothTime > 15) {
      for (int i = 0; i < ARR_LENGTH; i++) {
        receiveArr[i] = -1;
      }
    }
  }
}

int receive() {
  if (Serial1.available()) {
    lastCheck = Serial1.read();
    lastCheckTime = millis();
    return lastCheck;
  }

  // If recent, return last checked value
  if (millis() - lastCheckTime < 1000) {
    return lastCheck; 
  }

  return -1;
}

// === Accessor Functions ===
int getReceiveArr(int index) {
  return receiveArr[index];
}

int getSendArr(int index) {
  return sendArr[index];
}

void changeSendArr(int index, int data) {
  sendArr[index] = data;
}

int getArrLength() {
  return ARR_LENGTH;
}
