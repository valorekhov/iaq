#ifndef localui_h
#define localui_h

#ifndef bounce_h
  #include <Bounce.h>
#endif

#ifndef liquidcrystal_h
  #include <LiquidCrystal.h>
#endif

#include "State.h"
#include "ConfigMenu.h"

class LocalUI
{
public:
  void setup(State *_state, short button1Pin, short button2Pin);
  void update();  

private:
  ConfigMenu menu;
  void backlight(long thisSecond, int ambient);
  void readButtons(long thisSecond);
  void backlightOn(long thisSecond);
  void outputMotorStatus();
  void outputSensorStatus();
  Bounce *button1;
  Bounce *button2;
  bool button1Released;
  bool button2Released;
  LiquidCrystal *lcd;
  short photoDiodePin;
  long lcdLastOnSecond;
  short currentOutSensor;
  long lastSensorOutSecond;
  long lastKeyPressSecond;

  State *state;
};

#endif /*localui_h*/
