#ifndef localui_h
#define localui_h

#ifndef bounce_h
  #include <Bounce.h>
#endif

#ifndef liquidcrystal_h
  #include <LiquidCrystal.h>
#endif

#include "State.h"

class LocalUI
{
public:
  void setup(short button1Pin, short button2Pin);
  void update(State *state);  

private:
  void backlight(long thisSecond, int ambient);
  void readButtons();
  void outputMotorStatus(State *state);
  void outputSensorStatus(State *state);
  Bounce *button1;
  Bounce *button2;
  LiquidCrystal *lcd;
  short photoDiodePin;
  long lcdLastOnSecond;
  short currentOutSensor;
  long lastSensorOutSecond;
};

#endif /*localui_h*/
