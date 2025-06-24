#include "core_pins.h"
#include "DFUltraTrig.h"

#define VELOCITY_TEMP(temp) ((331.5 + 0.6 * (float)(temp)) * 100 / 1000000.0)  // cm/us

#define ULTRA_NUM 4

// Ultrasonic pins:      Front, Right, Back, Left
int trigechoPin[ULTRA_NUM] = {A9, A13, A8, A12};

int pastReadings[5] = {0,0,0,0};

uint16_t distance;
uint32_t pulseWidthUs;

long lastProcessTime = 0;
int ultraCount = 0;

// Distance readings for each sensor
int ultraReadings[ULTRA_NUM] = {0, 0, 0, 0};

// === Trigger and Read One Ultrasonic Sensor ===
void processDFUltraTrig() {
  if (millis() - lastProcessTime > 3) {
    pinMode(trigechoPin[ultraCount], OUTPUT);
    digitalWrite(trigechoPin[ultraCount], LOW);
    delayMicroseconds(2);  // Ensure proper LOW before triggering

    // Trigger pulse
    digitalWrite(trigechoPin[ultraCount], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigechoPin[ultraCount], LOW);

    // Set to input and measure echo duration
    pinMode(trigechoPin[ultraCount], INPUT);
    pulseWidthUs = pulseIn(trigechoPin[ultraCount], HIGH, 40000);

    // Calculate distance (cm), assume 20°C if no temp sensor
    distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;

    // Store result
    ultraReadings[ultraCount] = distance;

    // Update timing and cycle to next sensor
    lastProcessTime = millis();
    ultraCount = (ultraCount + 1) % ULTRA_NUM;
  }
}

// === Accessors ===
int getUltra(int which) {
  return ultraReadings[which];
}

int getUltraFront() { return analogRead(trigechoPin[0]); }
int getUltraRight() { 
  // int maxVal;
  // for (int i = 0; i < 4; ++i) {
  //   if (pastReadings[i] > maxVal) {
  //     maxVal = lastReadings[which][i];
  //   }
  // }
  return analogRead(trigechoPin[1]); 
}
int getUltraBack() { return analogRead(trigechoPin[2]); }
int getUltraLeft() { return analogRead(trigechoPin[3]); }

int getUltraFrontCM() { return analogRead(trigechoPin[0])*0.427304-4.43738; }
int getUltraRightCM() { 
  // int maxVal;
  // for (int i = 0; i < 4; ++i) {
  //   if (pastReadings[i] > maxVal) {
  //     maxVal = lastReadings[which][i];
  //   }
  // }
  return analogRead(trigechoPin[1])*0.427304-4.43738; 
}
int getUltraBackCM() { return analogRead(trigechoPin[2])*0.427304-4.43738; }
int getUltraLeftCM() { return analogRead(trigechoPin[3])*0.427304-4.43738; }

// int getUltraFront() { return getUltra(0); }
// int getUltraRight() { return getUltra(1); }
// int getUltraBack()  { return getUltra(2); }
// int getUltraLeft()  { return getUltra(3); }
