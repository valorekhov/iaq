#ifndef configmenu_h
#define configmenu_h

#ifndef liquidcrystal_h
  #include <LiquidCrystal.h>
#endif

#include "State.h"

#define MenuRun 0
#define MenuIntOn 1
#define MenuIntOff 2
#define MenuDefSpeed 3
#define MenuCurrentSpeed 4
#define MenuTurnOff 5

class ConfigMenu
{
public:
  void setup(State * _state, LiquidCrystal* _lcd){
    lcd = _lcd;
    state = _state;
    
    topMenu[MenuRun] = "Run";
    topMenu[MenuIntOn] = "Periodic On";
    topMenu[MenuIntOff] = "Periodic Off";
    topMenu[MenuDefSpeed] = "Default Speed";
    topMenu[MenuCurrentSpeed] = "Current Speed";
    topMenu[MenuTurnOff] = "Turn Off";    
  }
  
  bool isInMenu(){
      return menuLevel > 0;
  }
  
  void button1();
  void button2();
  void leaveMenu();

private:
  void enterMenu();
  void showTopMenu();
  void showRunMenu(bool advance);
  void initBottomMenu();
  void showIntermittentMenu(bool advance);
  void showSpeedMenu(bool advance);
  void showHrsSelection(char* defaultSetting);
  LiquidCrystal* lcd;
  char *topMenu[6];
  short menuLevel;
  short topMenuIdx;
  short bottomMenuIdx;  
  State * state;
};

#endif /*configmenu_h*/
