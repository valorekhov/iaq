#ifndef state_h
#define state_h

#include <Arduino.h>

class State
{
public:
  State(short motorControlPin, short motorLedPin){
    motorPin = motorControlPin;
    ledPin = motorLedPin;
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

  /*PROGMEM*/ int intermittentOn;
  /*PROGMEM*/ int intermittentOff;
  int intervalCountdown;
  int ambient;
  float* tempValues;
  float* humidityValues;
  
  void decrementCountdown(){
    if (intervalCountdown!=0xFFFF)
      if (intervalCountdown >0){
        intervalCountdown--;
        if (intervalCountdown<=0){
          intervalCountdown = 0xFFFF;
          motorOff();
        }
      }
  }
  
  short getSensorCount(){
    return sensorNum;
  }
    
  void motorOn(short level, int duration){
     intervalCountdown = duration;
     motorLevel = level;
     if (level == 0){
         motorOff();
     } else {
       digitalWrite(ledPin, HIGH);
       analogWrite(motorPin, 255);
     }
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
  int sensorNum;
  short motorLevel;
  short motorPin;
  short ledPin;
};

#endif /*state_h*/
