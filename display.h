#ifndef DISPLAY_H
#define DISPLAY_H

#include "func.h"
#include "button.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ------------ OLED hardware constants ----------- */
#define SCREEN_W     128
#define SCREEN_H      64
#define OLED_RESET    -1
#define OLED_ADDR   0x3C

/* Which side the robot is attacking */

void displaySetup();
void displayUpdate();
void drawHomePage();
void drawSensorPage();
void centerPrint(const String& txt, int y);

#endif