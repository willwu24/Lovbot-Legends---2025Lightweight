#include "core_pins.h"
#include "DFUltraTrig.h"

#define VELOCITY_TEMP(temp) ((331.5 + 0.6 * (float)(temp)) * 100 / 1000000.0)  // cm/us

#define ULTRA_NUM 4

// Ultrasonic pins:      Front, Right, Back, Left
int trigechoPin[ULTRA_NUM] = {A9, A13, A7, A12};

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

// int getUltraFront() { return analogRead(trigechoPin[0]); }
// int getUltraRight() { 
//   // int maxVal;
//   // for (int i = 0; i < 4; ++i) {
//   //   if (pastReadings[i] > maxVal) {
//   //     maxVal = lastReadings[which][i];
//   //   }
//   // }
//   return analogRead(trigechoPin[1]); 
// }
// int getUltraBack() { return analogRead(trigechoPin[2]); }
// int getUltraLeft() { return analogRead(trigechoPin[3]); }

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

int getUltraFront() { return analogRead(trigechoPin[0]); }
int getUltraRight() { return analogRead(trigechoPin[1]); }
int getUltraBack()  { return analogRead(trigechoPin[2]); }
int getUltraLeft()  { return analogRead(trigechoPin[3]); }

double degToRad(double deg) {
    return deg * PI / 180.0;
}



/* ─────────────────────────  USER SETTINGS  ───────────────────────── */
#define TRIM_WIN 5            // window length (must be 5 here)

/* ─────────────  PER-SENSOR STATE (ring buffers + counters)  ───────── */
static int  bufFront[TRIM_WIN],  idxFront = 0,  cntFront = 0;
static int  bufBack [TRIM_WIN],  idxBack  = 0,  cntBack  = 0;
static int  bufLeft [TRIM_WIN],  idxLeft  = 0,  cntLeft  = 0;
static int  bufRight[TRIM_WIN],  idxRight = 0,  cntRight = 0;

/* ─────────────  HELPER: trimmed mean of up to 5 ints  ─────────────── */
static int trimmedMean5(const int *arr, int n)
{
    int minV = arr[0], maxV = arr[0], sum = arr[0];
    for (int i = 1; i < n; ++i) {
        int v = arr[i];
        if (v < minV) minV = v;
        if (v > maxV) maxV = v;
        sum += v;
    }
    if (n < 3)                              // not enough samples yet
        return sum / n;                     // simple average

    sum -= (minV + maxV);                   // drop extremes
    return sum / (n - 2);                   // mean of middle 3
}

/* ─────────────  PUBLIC: SMOOTHED ACCESSORS  ───────────────────────── */
int getUltraFrontSmooth()
{
    int raw = getUltraFront();              // original sensor reading
    bufFront[idxFront] = raw;
    idxFront = (idxFront + 1) % TRIM_WIN;
    if (cntFront < TRIM_WIN) cntFront++;
    return trimmedMean5(bufFront, cntFront);
}

int getUltraBackSmooth()
{
    int raw = getUltraBack();
    bufBack[idxBack] = raw;
    idxBack = (idxBack + 1) % TRIM_WIN;
    if (cntBack < TRIM_WIN) cntBack++;
    return trimmedMean5(bufBack, cntBack);
}

int getUltraLeftSmooth()
{
    int raw = getUltraLeft();
    bufLeft[idxLeft] = raw;
    idxLeft = (idxLeft + 1) % TRIM_WIN;
    if (cntLeft < TRIM_WIN) cntLeft++;
    return trimmedMean5(bufLeft, cntLeft);
}

int getUltraRightSmooth()
{
    int raw = getUltraRight();
    bufRight[idxRight] = raw;
    idxRight = (idxRight + 1) % TRIM_WIN;
    if (cntRight < TRIM_WIN) cntRight++;
    return trimmedMean5(bufRight, cntRight);
}
