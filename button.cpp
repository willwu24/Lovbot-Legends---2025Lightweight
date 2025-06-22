#include "button.h"
#include "player.h"

const int TOTAL        = 5;      // number of buttons
const int TOTAL_STATE  = 2;      // for your 0/1 toggle
const unsigned long LONG_PRESS_MS = 100;  // adjust to taste
bool scanning = false;
bool prevButton = false;

/* -------------- internal state ---------------- */
int  buttonState[TOTAL]            = {0};   // your old toggle
int  previousButtonPressed[TOTAL]  = {0};   // last raw level
bool longPressed[TOTAL]            = {0};   // long-press flags

unsigned long pressStart[TOTAL]    = {0};   // millis() when press began

/* ----------- update all buttons (call every loop) --------------- */
void updateButton()
{
  for (int i = 0; i < TOTAL; ++i)
  {
    /* active-LOW: pressed = 1, released = 0 */
    int pressedNow = !digitalRead(BUTTON_PIN[i]);

    /* rising edge → remember the timestamp */
    if (pressedNow == 1 && previousButtonPressed[i] == 0)
    {
      pressStart[i] = millis();
    }

    /* if held long enough, set long-press flag */
    if (pressedNow == 1 &&
        (millis() - pressStart[i]) >= LONG_PRESS_MS)
    {
      longPressed[i] = true;
    }
    else if (pressedNow == 0)              // released → clear flag
    {
      longPressed[i] = false;
      pressStart[i]  = 0;
    }

    /* keep your original 0/1 toggle logic */
    if (pressedNow == 1 && previousButtonPressed[i] == 0)
    {
      buttonState[i] = (buttonState[i] + 1) % TOTAL_STATE;
    }

    previousButtonPressed[i] = pressedNow;
  }
}

/* ------------- public helpers ----------------- */
bool buttonPressed(int which)        { return longPressed[which]; }
bool getButtonState(int which)       { return buttonState[which]; }
bool buttonLongPressed(int which)    { return longPressed[which]; }

void clearButtonState()
{
  for (int i = 0; i < TOTAL; ++i) buttonState[i] = 0;
}

/* ------------- one-time setup ----------------- */
void setupButton()
{
  for (int i = 0; i < TOTAL; ++i)
  {
    pinMode(BUTTON_PIN[i], INPUT_PULLUP);   // stable HIGH when idle
  }
}

/* ------------- example actions ---------------- */
void checkButton()
{

  /* Hold button 3 for >500 ms to toggle attack mode */
  // if (buttonPressed(3))
  // {
  //   setAttackMode(getAttackMode() == YELLOW ? BLUE : YELLOW);
  // }
  // setLEDState(3, getAttackMode() == YELLOW ? 1 : 0);

  /* Hold button 2 for >500 ms to calibrate */
  if (buttonPressed(3)&&!prevButton)
  {
    scanning = !scanning;
  }
  if(scanning){
    Serial.println("calibrating threshold");
    calibrateThreshold();
  }
  setLEDState(3, scanning ? 1:0);
  prevButton = buttonPressed(3);
}