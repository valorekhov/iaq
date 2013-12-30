#ifndef serialinterface_h
#define serialinterface_h

#include "State.h"
#include <Arduino.h>

class SerialInterface
{
public:
  void setup(State * stateObj){
        state = stateObj;
        keyNameTemp = "TempX";
        keyNameHumidity = "HumidityX";
  }
  void monitorSerial();
  void reportSensors();
  
private:
  void printValue(String name, int value);
  void printValue(String name, float value);

  char serialData[256]; // Allocate some space for incoming serial strings
  short serialIndex;
  State * state;
  char *keyNameTemp;
  char *keyNameHumidity;
};

#endif /*serialinterface_h*/
