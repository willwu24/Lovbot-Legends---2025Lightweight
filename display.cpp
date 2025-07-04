/*
 *  display.cpp  – 5-page UI (re-mapped buttons, smaller dots)
 *  --------------------------------------------------------------
 *  Page 0 : Home banner
 *  Page 1 : Ultrasonic
 *  Page 2 : Light-Ring (32)
 *  Page 3 : Compound-Eye (18)
 *  Page 4 : Compass
 */

#include "display.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <math.h>

/* ───────── OLED & basic state ───────── */
static Adafruit_SSD1306 oled(SCREEN_W, SCREEN_H, &Wire, OLED_RESET);

static int page = 0;                      // 0-4
static int lastAttackMode = -1;                   // BLUE / YELLOW banner

/* demo sensor data – swap with real values */

/* long-press buttons */
static bool pB0=false, pB1=false, pB2=false;

/* redraw tracking */
static int lastPage=-1, lastSide=-1;

/* ───── helpers ───── */
static void titleBar(const char* ttl);
static void centerPrint(const String& s,int y);
static void drawHome(),drawUltra(),drawLight(),drawEye(),drawCompass();

/* =========================================================== */
void displaySetup()
{
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled.clearDisplay();
  oled.display();
}

/* =========================================================== */
void displayUpdate()
{
  bool b0=buttonPressed(2);   // toggle BLUE / YELLOW
  bool b1=buttonPressed(0);   // -- now “previous”
  bool b2=buttonPressed(4);   // -- now “next”

  // if(b0 && !pB0) side = (side==BLUE)?YELLOW:BLUE;
  if (b0 && !pB0){
    int current = getAttackMode();
    setAttackMode(current==BLUE?YELLOW:BLUE);
    
  }

  if(b1 && !pB1) page = (page+4)%5;    //  –1  (left)
  if(b2 && !pB2) page = (page+1)%5;    //  +1  (right)

  pB0=b0; pB1=b1; pB2=b2;

  int currentAttackMode = getAttackMode();


  bool forceRefresh = page>0;
  if(page!=lastPage || currentAttackMode!=lastAttackMode || forceRefresh)
  {
    Serial.print("refresh");
    oled.clearDisplay();
    switch(page){
      case 0: titleBar("HOME"   ); drawHome();    break;
      case 1: titleBar("DIRECTION "); drawCompass(); break;
      case 2: titleBar("LIGHT"  ); drawLight();   break;
      case 3: titleBar("EYE"    ); drawEye();     break;
      case 4: titleBar("ULTRA"  ); drawUltra();   break;
    }
    oled.display();
    lastPage=page; lastAttackMode = currentAttackMode;
  }
}

/* ===========================================================
 *                     TITLE BAR (6 px)
 * =========================================================== */
static void titleBar(const char* ttl)
{
  oled.fillRect(0,0,128,6,SSD1306_BLACK);
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(2,0); oled.print(ttl);

  for(int i=0;i<5;++i){
      int8_t x=108+i*4;
      (i==page)? oled.fillCircle(x,3,1,SSD1306_WHITE)
               : oled.drawPixel (x,3,SSD1306_WHITE);
  }
}

/* ===========================================================
 *                          PAGE 0
 * =========================================================== */
static void drawHome()
{
  int side = getAttackMode();

  if(side==BLUE){
      oled.drawRoundRect(4,10,120,24,4,SSD1306_WHITE);
      oled.setTextColor(SSD1306_WHITE);
  }else{
      oled.fillRoundRect(4,10,120,24,4,SSD1306_WHITE);
      oled.setTextColor(SSD1306_BLACK);
  }

  oled.setTextSize(2);
  if (side==BLUE){
    oled.setCursor(40, 16); oled.print("BLUE");
  }
  else{
    oled.setCursor(28, 16); oled.print("YELLOW");
  }
  // centerPrint(,18);

  if (getRobotRole() == OFFENSE){
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    centerPrint("OFFENSE",42);
  }
  else
  {
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    centerPrint("DEFENSE",42);
  }
}

/* ===========================================================
 *                PAGE 1 : ULTRASONIC  (tidy)
 * =========================================================== */
static void drawUltra()
{
  int usFront=getUltraFront(), usBack=getUltraBack(), usLeft=getUltraLeft(), usRight=getUltraRight();
    /* frame (same size as before) */
    oled.drawRoundRect(0, 8, 128, 56, 4, SSD1306_WHITE);

    oled.setTextSize(1);

    /* ---------- FRONT (top-centre) ---------- */
    centerPrint(usFront, 12);

    /* ---------- BACK  (bottom-centre) ------- */
    centerPrint(usBack, 52);

    /* ---------- LEFT  (middle-left) --------- */
    oled.setCursor(4, 30);
    oled.print(usLeft);

    /* ---------- RIGHT (middle-right) -------- */
    String rightTxt = String("R ") + String(usRight) + "cm";
    oled.setCursor(128 - 6 * rightTxt.length() - 4, 30);
    oled.print(usRight);
}


/* ===========================================================
 *        PAGE 2 : LIGHT-RING  (dots back to radius 2 px)
 * =========================================================== */
static void drawLight()
{
    bool ring[32];

    getWhiteSensor(ring);
    
    int   ang = getWhiteAngleOffense();              // whiteAngle
    float avg = getSensitivity(), mag = getMagnitude();

    const int16_t cx = 32, cy = 38, R = 23;   // ring radius fits under title
    const uint8_t rDot = 2;                   // **back to original size**

    // oled.drawCircle(cx, cy, R, SSD1306_WHITE);

    for (uint8_t i = 0; i < 32; ++i) {
        float a = (TWO_PI * i / 32.0f) - PI / 2.0f;
        int16_t x = cx + R * cosf(a);
        int16_t y = cy + R * sinf(a);
        ring[i] ? oled.fillCircle(x, y, rDot, SSD1306_WHITE)
                : oled.drawCircle(x, y, rDot, SSD1306_WHITE);
    }

    float a = (ang - 90) * DEG_TO_RAD;
    oled.drawLine(cx, cy,
                  cx + (R - 3) * cosf(a),
                  cy + (R - 3) * sinf(a),
                  SSD1306_WHITE);

    oled.setTextSize(1);
    oled.setCursor(70, 18); oled.print("T "); oled.print(avg, 2);
    oled.setCursor(70, 30); oled.print("A "); oled.print(ang);
    oled.setCursor(70, 42); oled.print("M "); oled.print(mag, 2);
}

/* ===========================================================
 *       PAGE 3 : COMPOUND-EYE  (dots back to radius 3 px)
 * =========================================================== */
static void drawEye()
{
    int   ang  = getEyeAngle();            // eyeAngle
    float dist = getEyeValue();      // eyeDistance 0-1
    uint8_t hit = (ang % 360) / 20;

    const int16_t cx = 32, cy = 35, R = 23;   // ring radius fits under title
    const uint8_t rDot = 3;                   // **back to original size**

    // oled.drawCircle(cx, cy, R, SSD1306_WHITE);

    for (uint8_t i = 0; i < 18; ++i) {
        float a = (TWO_PI * i / 18.0f) - PI / 2.0f;
        int16_t x = cx + R * cosf(a);
        int16_t y = cy + R * sinf(a);
        (i == hit) ? oled.fillCircle(x, y, rDot, SSD1306_WHITE)
                   : oled.drawCircle(x, y, rDot, SSD1306_WHITE);
    }

    float a = (ang - 90) * DEG_TO_RAD;
    int16_t L = (R - 3) * constrain(dist, 0.0f, 1.0f);
    int16_t xt = cx + L * cosf(a);
    int16_t yt = cy + L * sinf(a);
    oled.drawLine(cx, cy, xt, yt, SSD1306_WHITE);

    oled.setTextSize(1);
    oled.setCursor(70, 26); oled.print("A "); oled.print(ang);
    oled.setCursor(70, 38); oled.print("D "); oled.print(dist, 2);
}


/* ===========================================================
 *      PAGE 4 : Compass – main disk with rotating gap
 * =========================================================== */
/* ===========================================================
 *  PAGE 4 : Compass — heading value in centre, motion vector
 * =========================================================== */
static void drawCompass()
{
    /* -------- replace with live data -------- */
    int headingCW = getCompass();        // 0-359°, clockwise, 0° = north
    int dir       = getDir();       // motion direction (0-359°, CW, 0° = north)
    int speed     = getSpeed();        // 0-50  (vector length)

    /* -------- geometry -------- */
    const int16_t cx  = 32, cy = 38;  // robot centre
    const int16_t R   = 26;           // main disk radius
    const int16_t Rgap = 10;          // gap disk radius
    const int16_t vecMax = R - 2;     // max vector length

    /* -------- draw filled disk -------- */
    oled.fillCircle(cx, cy, R, SSD1306_WHITE);

    /* -------- carve heading gap -------- */
    float headRad = headingCW * DEG_TO_RAD;
    float hx =  sinf(headRad);
    float hy = -cosf(headRad);        // CW system: +y down
    oled.fillCircle(cx + R * hx, cy + R * hy, Rgap, SSD1306_BLACK);

    /* -------- heading value (black) in centre -------- */
    oled.setTextSize(1);
    String headStr = String(headingCW);
    int16_t hxText = cx - (6 * headStr.length()) / 2;
    oled.setTextColor(SSD1306_BLACK);
    oled.setCursor(hxText, cy - 4);
    oled.print(headStr); 
    oled.setTextColor(SSD1306_WHITE);

    /* -------- motion vector (dir / speed) ------------ */
    float dirRad = dir * DEG_TO_RAD;
    float dx =  sinf(dirRad);
    float dy = -cosf(dirRad);
    int16_t L = vecMax * constrain(speed, 0, 50) / 50.0f;

    int16_t tipX = cx + L * dx;
    int16_t tipY = cy + L * dy;
    oled.drawLine(cx, cy, tipX, tipY, SSD1306_BLACK);

    /* arrowhead */
    float leftA  = atan2f(dy, dx) + PI / 9;
    float rightA = atan2f(dy, dx) - PI / 9;
    int16_t hx1 = tipX + (int16_t)(-4 * cosf(leftA));
    int16_t hy1 = tipY + (int16_t)(-4 * sinf(leftA));
    int16_t hx2 = tipX + (int16_t)(-4 * cosf(rightA));
    int16_t hy2 = tipY + (int16_t)(-4 * sinf(rightA));
    oled.drawLine(tipX, tipY, hx1, hy1, SSD1306_BLACK);
    oled.drawLine(tipX, tipY, hx2, hy2, SSD1306_BLACK);

    /* -------- dir / speed on the right ---------------- */
    oled.setCursor(74, 26); oled.print("Dir "); oled.print(dir);
    oled.setCursor(74, 36); oled.print("Spd "); oled.print(speed);
}








/* ===========================================================
 *                    centred text helper
 * =========================================================== */
static void centerPrint(const String& s,int y)
{
  int16_t x=(SCREEN_W-6*s.length())/2;
  oled.setCursor(x,y); oled.print(s);
}