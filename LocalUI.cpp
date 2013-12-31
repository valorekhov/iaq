#include "LocalUI.h"
#include <Arduino.h>


void LocalUI::setup(State *_state, short button1Pin, short button2Pin)
{
  state = _state;
  pinMode(button1Pin, INPUT);     
  pinMode(button2Pin, INPUT);
  
  button1 = new Bounce(button1Pin, 50);
  button2 = new Bounce(button2Pin, 50);
  
  lcd = new LiquidCrystal(0);
  lcd->begin(16, 2);
  lcd->setBacklight(HIGH);
  
  menu.setup(_state, lcd);
}

void LocalUI::update(){
    long thisSecond = millis() / 1000;

    LocalUI::readButtons(thisSecond);
    if (!menu.isInMenu()){
      LocalUI::outputMotorStatus();
      
      if (thisSecond % 5 == 0 && thisSecond != lastSensorOutSecond){
            LocalUI::outputSensorStatus();
            lastSensorOutSecond = thisSecond;
      }
      
      if (thisSecond % 10 == 0){
        LocalUI::backlight(thisSecond, state->ambient);
      }
    }
}

void LocalUI::outputMotorStatus(){
      lcd->setCursor(0, 0);
      int motor = state->getMotor();
  
      if (motor > 0){
        lcd->print("On:");
        lcd->print(motor);
        if (state->intervalCountdown > 0 && state->intervalCountdown!=0xFFFF){
            lcd->print(" ");  
            if (state->intervalCountdown > 3660){
                float hours = (float)state->intervalCountdown / 3660;
                lcd->print(hours);
                lcd->print(" hrs");
            } else if (state->intervalCountdown > 180){
                int minutes = state->intervalCountdown / 60;
                lcd->print(minutes);
                lcd->print(" min");
            } else 
            {
                lcd->print(state->intervalCountdown);
                lcd->print(" sec");
            }
        }
      } else
      {
        lcd->print("Off");
      }
      lcd->print("             ");
}

void LocalUI::outputSensorStatus(){
      lcd->setCursor(0, 1);
      int sensorCount = state->getSensorCount();
      int tries = 0;
     
      while(isnan(state->tempValues[currentOutSensor]) && isnan(state->humidityValues[currentOutSensor]) && tries++ < sensorCount){
        if (++currentOutSensor >= sensorCount)
            currentOutSensor = 0;
      }      
      if (tries < sensorCount){
        if (currentOutSensor==0)
           lcd->print("Out ");
        else
        {
           lcd->print("S#");
           lcd->print(currentOutSensor);
           lcd->print(" ");
        }        
        if (!isnan(state->tempValues[currentOutSensor])){
             lcd->print("T=");
             lcd->print((int)state->tempValues[currentOutSensor]);
             lcd->print("C ");
        }
        if (!isnan(state->humidityValues[currentOutSensor])){
             lcd->print("RH=");
             lcd->print((int)state->humidityValues[currentOutSensor]);
             lcd->print("%");
        }
      }
      lcd->print("       ");
      if (++currentOutSensor >= sensorCount)
            currentOutSensor = 0;

}


void LocalUI::backlight(long thisSecond, int ambient){
    if (ambient > 600){                  //Unit is to be installed in a basement with no natural light
      backlightOn(thisSecond);
    } else if (ambient < 400){             //Turn off backlight if lights are off and no one is around after about a min. 400..600 is the deadband to prevent jitter
      if (thisSecond > lcdLastOnSecond + 60){
        lcd->setBacklight(LOW);
      }
    }
}

void LocalUI::backlightOn(long thisSecond){
      lcdLastOnSecond = thisSecond;
      lcd->setBacklight(HIGH);
}

void LocalUI::readButtons(long thisSecond){
  button1->update();
  button2->update();
  if (button1Released && button1->read() == HIGH)
  {
    backlightOn(thisSecond);
    menu.button1();
    button1Released = false;
  } else if (button1->read() == LOW)
  {
    button1Released = true;
  }
  if (button2Released && button2->read() == HIGH)
  {
    backlightOn(thisSecond);
    menu.button2();
    button2Released = false;
  } else if (button2->read() == LOW){
    button2Released = true;
  }
}



