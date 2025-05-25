#include "display.h"

void displayDistance() {
  Serial.print("Left Distance: ");
  Serial.print(getUltraLeft());
  Serial.print("  |  Right Distance: ");
  Serial.print(getUltraRight());
  Serial.print("  |  Front Distance: ");
  Serial.print(getUltraFront());
  Serial.print("  |  Back Distance: ");
  Serial.print(getUltraBack());
  Serial.print("   ");
}

void displayCompass() {
  Serial.print("Compass: ");
  Serial.print(getCompass());
  Serial.print("  |  Raw Compass: ");
  // Serial.print(getRawCompass()); // Uncomment if needed
  Serial.print("   ");
}

void displayCamera() {
  // Yellow blob information
  Serial.print("  |  Yellow Detected: ");
  Serial.print(yellowDetected());
  Serial.print("  |  Yellow Distance: ");
  Serial.print(getYellowDistance());
  Serial.print("  |  Yellow Center Angle: ");
  Serial.print(getYellowAngle());
  Serial.print("  |  Yellow Left Edge Angle: ");
  Serial.print(getYellowLeftEdgeAngle());
  Serial.print("  |  Yellow Right Edge Angle: ");
  Serial.print(getYellowRightEdgeAngle());

  // Blue blob information
  Serial.print("  |  Blue Detected: ");
  Serial.print(blueDetected());
  Serial.print("  |  Blue Distance: ");
  Serial.print(getBlueDistance());
  Serial.print("  |  Blue Center Angle: ");
  Serial.print(getBlueAngle());
  Serial.print("  |  Blue Left Edge Angle: ");
  Serial.print(getBlueLeftEdgeAngle());
  Serial.print("  |  Blue Right Edge Angle: ");
  Serial.print(getBlueRightEdgeAngle());
}
