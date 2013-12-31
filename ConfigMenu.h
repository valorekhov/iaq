#ifndef configmenu_h
#define configmenu_h

#ifndef liquidcrystal_h
  #include <LiquidCrystal.h>
#endif

#include "State.h"

class ConfigMenu
{
public:
  void setup(State * _state, LiquidCrystal* _lcd){
    lcd = _lcd;
    state = _state;
    
    topMenu[0] = "Run";
    topMenu[1] = "Intermittent On";
    topMenu[2] = "Intermittent Off";
    topMenu[3] = "Turn Off";
    
  }
  
  bool isInMenu(){
      return menuLevel > 0;
  }
  
  void button1();
  void button2();


private:
  void enterMenu();
  void showTopMenu();
  void showRunMenu();
  void showIntermittentMenu();
  void leaveMenu();
  LiquidCrystal* lcd;
  char *topMenu[4];
  short menuLevel;
  short topMenuIdx;
  short bottomMenuIdx;  
  State * state;
};

#endif /*configmenu_h*/
