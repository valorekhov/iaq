#include "ConfigMenu.h"
#include <math.h>

//The left button is used to navigate through choices
void ConfigMenu::button1(){
  if (!isInMenu())
    enterMenu();
  else if (menuLevel==1){
      if (++topMenuIdx >= 3) topMenuIdx=0;
      showTopMenu();
      
      if (topMenuIdx ==1 || topMenuIdx==2){
         int selection = topMenuIdx == 1 ? state->intermittentOn : state->intermittentOff;
         if (selection > 0)
         {
                 selection = log(selection / 1800) / log(2);
         } else
           selection = -1;
         bottomMenuIdx = selection;
         showIntermittentMenu();
      }
    }
  else if (menuLevel==2){
      switch(topMenuIdx){
          case 0:
             showRunMenu();
             break;
          case 1:
          case 2:
             showIntermittentMenu();
             break;
      };
  }  
}

//The right botton is used to commit actions. 
void ConfigMenu::button2(){
  int runtime;
  if ( (state->getMotor() && !isInMenu()) || (menuLevel ==1 && topMenuIdx==3)){
      state->intervalCountdown = 0;
      state->motorOff();
      leaveMenu();
  }
  else if (menuLevel ==1){
      menuLevel=2;
      if (topMenuIdx == 0)
        bottomMenuIdx=0;
      lcd->setCursor(0, 0);
      lcd->print(" ");
      lcd->setCursor(0, 1);
      lcd->print(">");
      button1();
  } else if (menuLevel ==2){
      runtime = pow(2, bottomMenuIdx -1) * 1800 ;
      switch(topMenuIdx){
            case 0:                    //Commit selection for the Run menu               
               if (bottomMenuIdx==0){   //0 selection is the "Always On" setting.
                  runtime = 0xFFFF;
               }
               state->motorOn(80, runtime);
               break;
            case 1:
            case 2:
               if (bottomMenuIdx==0){   //0 selection is the "Not Set" setting.
                  runtime = 0;
               }
               if (topMenuIdx == 1)
                 state->intermittentOn = runtime;
               else 
                 state->intermittentOff = runtime;
               break;
    }
    leaveMenu();
  }  
}

void ConfigMenu::enterMenu(){
  menuLevel=1;
  if (state->getMotor())
    topMenuIdx = 2;
  else
    topMenuIdx = 0;
    
  showTopMenu();
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


void ConfigMenu::showRunMenu(){
  if (++bottomMenuIdx > 6){
    bottomMenuIdx = 0;
  }

  lcd->setCursor(1, 1);
  if (bottomMenuIdx == 0){
    lcd->print("Always on");
  } else
  {
    float hrs = pow(2, bottomMenuIdx -1) * .5;
    lcd->print(hrs);
    lcd->print(" hours");
  }
  lcd->print("         ");
}

void ConfigMenu::showIntermittentMenu(){
  if (++bottomMenuIdx > 5){
    bottomMenuIdx = 0;
  }

  lcd->setCursor(1, 1);
  if (bottomMenuIdx == 0){
    lcd->print("Not set");
  } else {
    float hrs = pow(2, bottomMenuIdx -1) * .5;
    lcd->print(hrs);
    lcd->print(" hours");
  }

  lcd->print("        ");
}
