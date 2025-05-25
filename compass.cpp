#include "compass.h"

// --- Compass Configuration ---
long compass_max_value = 480;
int compass_type = 0;
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// --- Compass Tracking ---
int heading = 359;
int lastRawValue = 0;

// --- Write to BNO Register ---
void bno_write(uint8_t i2c_addr, uint8_t reg, uint8_t data) {
  Wire.beginTransmission(i2c_addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);  // send stop
}

// --- Get Difference Between Two Compass Angles ---
int getCompassDif(int target, int currentValue) {
  int diff = 0;

  if (target < 180) {
    if (currentValue > target + 180) {
      diff = -360 + currentValue - target;
    } else {
      diff = currentValue - target;
    }
  } else {
    if (currentValue > target - 180) {
      diff = currentValue - target;
    } else {
      diff = 360 - target + currentValue;
    }
  }

  return diff;
}

// --- Read Raw Compass Value ---
int getCompassRawValue() {
  int rawValue = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();

  if (abs(getCompassDif(lastRawValue, rawValue)) > 100) {
    heading = rawValue;
  }

  lastRawValue = rawValue;
  return rawValue;
}

// --- Get Adjusted Compass Heading (0-359) ---
int getCompass() {
  int rawDegree = getCompassRawValue();

  if (buttonPressed(1)) {
    heading = rawDegree;
  }

  int adjusted = (rawDegree - heading + 360) % 360;
  return adjusted;
}

// --- Compass Initialization ---
void setupCompass() {
  Serial.println("Setting compass type to BNO055");

  if (!bno.begin()) {
    Serial.println("Error: Failed to initialize BNO055");
    while (1);  // halt
  }

  bno.setExtCrystalUse(true);
  sensor_t sensor;
  bno.getSensor(&sensor);

  // Set to NDOF mode
  bno.setMode(0x0C);

  // Configure BNO registers
  bno_write(BNO_ADDR, PAGE_ID, 1);        // Register page 1
  bno_write(BNO_ADDR, ACC_CONFIG, 0x0D);  // Accel 2g range
  bno_write(BNO_ADDR, PAGE_ID, 0);        // Back to page 0
  bno_write(BNO_ADDR, OPR_MODE, 0x0C);    // Operating mode

  delay(10);  // Allow time for BNO to switch modes
}
