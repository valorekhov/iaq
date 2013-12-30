#include <Wire.h>
#include <Bounce.h>
#include <LiquidCrystal.h>

#include "DHT.h"
#include "State.h"
#include "LocalUI.h"
#include "SerialInterface.h"

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
#define MotorOnLedPin 13
#define Button1Pin 12
#define Button2Pin 11
#define Sensor1Pin 2
#define Sensor2Pin 3
#define Sensor3Pin 4
#define MotorPwmControlPin 9
#define PhotoDiodePin A0

//Sensor, state Values
State state(MotorPwmControlPin, MotorOnLedPin);
LocalUI ui;
SerialInterface serialiface;

DHT dht1;
DHT dht2;
DHT dht3;

#define SensorCount 3
DHT* sensors[SensorCount];

long thisSecond = -1;

// the setup routine runs once when you press reset:
void setup() {  
  state.setup(SensorCount + 1);      // One more sensor slot for outside temp, rh
  ui.setup(Button1Pin, Button2Pin);
  serialiface.setup(&state);
  
  pinMode(MotorOnLedPin, OUTPUT);
  pinMode(MotorPwmControlPin, OUTPUT);
  
  Serial.begin(115200);
  dht1.setup(Sensor1Pin); 
  dht2.setup(Sensor1Pin); 
  dht3.setup(Sensor1Pin); 
  
  sensors[0] = &dht1;
  sensors[1] = &dht2;
  sensors[2] = &dht3;
}

// the loop routine runs over and over again forever:
void loop() {
  long second = millis() / 1000;

  state.ambient = analogRead(PhotoDiodePin);

  serialiface.monitorSerial();
  ui.update(&state);  

  if (second >  thisSecond){
    state.decrementCountdown();
    thisSecond = second;

    if (second % 10 == 0){
      pollSensors();
    }   
  }  
  delay(250);  
}

void pollSensors(){
      int sensorCount = state.getSensorCount();
      
      for(int i=0; i<SensorCount; i++){
          DHT* dht = sensors[i]; 
          
          float temp = dht->getTemperature();
          float humidity = dht->getHumidity();
          
          if (!isnan(temp))
            state.tempValues[i+1] = temp;
          if (!isnan(humidity))
            state.humidityValues[i+1] = humidity;
      }     
      
      serialiface.reportSensors();
}



