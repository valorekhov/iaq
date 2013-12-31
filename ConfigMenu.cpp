#include "ConfigMenu.h"
#include <math.h>

void ConfigMenu::enterMenu(){
  menuLevel=1;
  if (state->getMotor())
    topMenuIdx = MenuCurrentSpeed;
  else
    topMenuIdx = MenuRun;
  showTopMenu();
  initBottomMenu();
}

//The left button is used to navigate through choices
void ConfigMenu::button1(){
  if (!isInMenu())
    enterMenu();
  else if (menuLevel==1){
      int stopAt = state->getMotor() ? MenuTurnOff : MenuDefSpeed;   //only show "Turn off", "Current Speed" options menu option if motor is running
      if (++topMenuIdx > stopAt) topMenuIdx=MenuRun;  
      showTopMenu();      
      initBottomMenu();
    }
  else if (menuLevel==2){        // Advance selesctions within the 2nd level menus
      switch(topMenuIdx){
          case MenuRun:
             showRunMenu(true);
             break;
          case MenuIntOn:
          case MenuIntOff:
             showIntermittentMenu(true);
             break;             
          case MenuDefSpeed:
          case MenuCurrentSpeed:
             showSpeedMenu(true);
             break;
      }
  }  
}

void ConfigMenu::initBottomMenu(){
  switch(topMenuIdx){
      case MenuRun:
            bottomMenuIdx=state->getIntermittentOn() > 0 && state->getIntermittentOff() ==0 ? 0 : 1;
                                  //Intermittent On is set while Int Off is 0 ==> motor will run always. Init to the corresponding selection
            showRunMenu(false);
          break;
       case MenuIntOn:
       case MenuIntOff:{
               int selection = topMenuIdx == MenuIntOn  ? state->getIntermittentOn() : state->getIntermittentOff();
               if (selection > 0)
               {
                       selection = log(selection / 1800) / log(2) +1;
               } 
               bottomMenuIdx = selection;
               showIntermittentMenu(false);
             }
           break;
        case MenuDefSpeed:
        case MenuCurrentSpeed:
               bottomMenuIdx = topMenuIdx == MenuDefSpeed ? state->getDefaultMotorLevel() : state->getMotor();
               if (bottomMenuIdx > 100)
                  bottomMenuIdx = 100;
               else if (bottomMenuIdx<0)
                  bottomMenuIdx = 0;
      
               bottomMenuIdx = (bottomMenuIdx - 20) / 10;            
      
               showSpeedMenu(false);
             break;
  }
}
//The right botton is used to commit actions. 
void ConfigMenu::button2(){
  int value;
  if ( (state->getMotor() && !isInMenu()) || (menuLevel ==1 && topMenuIdx==MenuTurnOff)){
      state->intervalCountdown = 0;
      state->motorOff();
      leaveMenu();
  }
  else if (menuLevel ==1){            //1st level menu was selected here. transition into 2nd level.
      menuLevel=2;
      lcd->setCursor(0, 0);
      lcd->print(" ");
      lcd->setCursor(0, 1);
      lcd->print(">");
      initBottomMenu();
      //button1();
  } else if (menuLevel ==2){          //Process hours-based selections for Run, Int On/Off menus here
      if (topMenuIdx <= MenuIntOff)
          value = ceil(pow(2, bottomMenuIdx -1)) * 1800 ;
      switch(topMenuIdx){
            case MenuRun:                    //Commit selection for the Run menu               
               if (bottomMenuIdx==0){        //0 selection is the "Always On" setting.
                  value = 0xFFFF;
               }
               state->motorOn(state->getDefaultMotorLevel(), value);
               break;
            case MenuIntOn:
            case MenuIntOff:
               if (bottomMenuIdx==0){       //0 selection is the "Not Set" setting.
                  value = 0;
               }
               if (topMenuIdx == MenuIntOn)
                 state->setIntermittentOn(value);
               else 
                 state->setIntermittentOff(value);
               break;
            case MenuDefSpeed:
            case MenuCurrentSpeed:
               value = bottomMenuIdx * 10 + 20;
               if (topMenuIdx == MenuDefSpeed)
                 state->setDefaultMotorLevel(value);
               else 
                 state->motorOn(value);
               break;
    }
    leaveMenu();
  }  
}

void ConfigMenu::leaveMenu(){
   lcd->clear();
   menuLevel = 0;
}

void ConfigMenu::showTopMenu(){
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print(">");
  lcd->print(topMenu[topMenuIdx]);
}


void ConfigMenu::showRunMenu(bool advance){
  if (advance && ++bottomMenuIdx > 6){
    bottomMenuIdx = 0;
  }
  showHrsSelection("Always on");
}

void ConfigMenu::showIntermittentMenu(bool advance){
  if (advance && ++bottomMenuIdx > 5){
    bottomMenuIdx = 0;
  }
  showHrsSelection("Not set");
}


void ConfigMenu::showHrsSelection(char* defaultSetting){
  lcd->setCursor(1, 1);
  if (bottomMenuIdx == 0){
    lcd->print(defaultSetting);
  } else {
    float hrs = pow(2, bottomMenuIdx -1) * .5;
    lcd->print(hrs);
    lcd->print(" hours");
  }
  lcd->print("        ");
}

void ConfigMenu::showSpeedMenu(bool advance){
  if (advance && ++bottomMenuIdx > 8){
    bottomMenuIdx = 0;
  }
  
  lcd->setCursor(1, 1);
  int spd = bottomMenuIdx * 10 + 20;
  lcd->print(spd);
  lcd->print("%");
  lcd->print("        ");
}

