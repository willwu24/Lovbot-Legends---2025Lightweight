#include "display.h"

void displayDistance(){
  Serial.print("left distance: ");
  Serial.print(getUltraLeft());
  Serial.print("  |  Right distance: ");
  Serial.print(getUltraRight());
  Serial.print("  |  Front distance: ");
  Serial.print(getUltraFront());
  Serial.print("  |  Back distance: ");
  Serial.print(getUltraBack());
  Serial.print("   ");
}

void displayCoordinate(){
  Serial.print("x: ");
  Serial.print(getX());
  Serial.print("  |  y: ");
  Serial.print(getY());
  Serial.print("  |  x blocked: ");
  Serial.print(getXBlocked());
  Serial.print("  |  y blocked: ");
  Serial.print(getYBlocked());
  Serial.print("   ");
}

void displayCompass(){
  Serial.print("compass: ");
  Serial.print(getCompass());
  Serial.print("  |  RawCompass: ");
  // Serial.print(getRawCompass());
  Serial.print("   ");
}

void displayCamera(){
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
