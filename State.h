#ifndef state_h
#define state_h

#include <Arduino.h>
#include <EEPROM.h>


class State
{
public:
  State(short motorControlPin, short motorLedPin){
    motorPin = motorControlPin;
    ledPin = motorLedPin;
    
    defaultMotorLevel = EEPROM.read(0);
    periodicOn = readEepromInt(1);
    periodicOff = readEepromInt(5);
    
    if (defaultMotorLevel <= 0)
       defaultMotorLevel = 80;
       
    if (periodicOn<0){
       setIntermittentOn(0);
    }
    if (periodicOff<0){
       setIntermittentOff(0);
    }
    if (periodicOn > 0)
      motorOn(defaultMotorLevel, periodicOff > 0 ? periodicOn : 0xFFFF);  //If periodic off was not specified, run in "always on" mode 
  }
  
  void setup(short sensorCount){
      sensorNum = sensorCount;
      tempValues = new float[sensorCount];
      humidityValues = new float[sensorCount];
      
      for(int i=0; i<sensorCount; i++){
          tempValues[i] = NAN;
          humidityValues[i] = NAN;
      }
  }

  int intervalCountdown;
  int ambient;
  float* tempValues;
  float* humidityValues;
  
  void decrementCountdown(){
    if (intervalCountdown!=0xFFFF && intervalCountdown >0){
        intervalCountdown--;
        
        if (intervalCountdown<=0){
          if (motorLevel > 0) {  
           if (periodicOn > 0 && periodicOff==0){  
              //This state corresponds to "Always On" operation. Intermittent On has been specified w/o corresponing off cycle.
              //Reset the counter and do not turn motor off 
              intervalCountdown = periodicOn;
           } else
           {
               //motor is currently running, transition to Off
               intervalCountdown = periodicOff > 0 ? periodicOff : 0xFFFF;
               motorOff();
           }
          } else
          {
              motorOn(defaultMotorLevel, periodicOn > 0 ? periodicOn : 0xFFFF);  
          }
        }
    }
  }
  
  short getSensorCount(){
    return sensorNum;
  }
  
  int getIntermittentOff(){
    return periodicOff;
  }
  
  int getIntermittentOn(){
    return periodicOn;
  }
  
  void setIntermittentOff(int value){
    //Stored as EEPROM.read(5) << 24 | EEPROM.read(6) << 16 | EEPROM.read(7) << 8 | EEPROM.read(8);
    periodicOff = value;
    writeEepromInt(value, 5);
  }
  
  void setIntermittentOn(int value){
    //Stored as  EEPROM.read(1) << 24 | EEPROM.read(2) << 16 | EEPROM.read(3) << 8 | EEPROM.read(4);
    periodicOn = value;
    writeEepromInt(value, 1);
  }

  char getDefaultMotorLevel(){
    return defaultMotorLevel;
  }  
  
  void setDefaultMotorLevel(char value){
    defaultMotorLevel = value;
    EEPROM.write(0, value);
  }
  
  void motorOn(char level){ 
     if (level > 100)
       level = 100;    
     motorLevel = level;
     if (level == 0){
         motorOff();
     } else {
       digitalWrite(ledPin, HIGH);
       level = 0xFF * (level/100.0);
       analogWrite(motorPin, level);
     }
  }

 
  void motorOn(char level, int duration){
     intervalCountdown = duration;
     motorOn(level);    
  }
  
  void motorOff(){
     motorLevel = 0;
     digitalWrite(ledPin, LOW);
     analogWrite(motorPin, 0);
  }
  
  short getMotor(){
      return motorLevel;
  }
  
private:

  void writeEepromInt(int value, char idx){
    EEPROM.write(idx+3, value & 0xFF); 
    EEPROM.write(idx+2, (value >> 8) & 0xFF);
    EEPROM.write(idx+1, (value >> 16) & 0xFF); 
    EEPROM.write(idx, (value >> 24) & 0xFF);
  }
  
  int readEepromInt(char idx){
    return EEPROM.read(idx) << 24 | EEPROM.read(idx+1) << 16 | EEPROM.read(idx+2) << 8 | EEPROM.read(idx+3);
  }

  char sensorNum;
  char motorLevel;
  char motorPin;
  char ledPin;

  char defaultMotorLevel;
  int periodicOn;
  int periodicOff;
};

#endif /*state_h*/
