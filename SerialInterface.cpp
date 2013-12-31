#include "SerialInterface.h"

void SerialInterface::monitorSerial(void) {  
  char *tok;
  char *p;
  char inChar=-1;
  
  while (Serial.available() > 0 && serialIndex < 255) 
  {
       inChar = Serial.read(); // Read a character
       serialData[serialIndex] = inChar;
       serialData[++serialIndex] = '\0'; // Null terminate the string
       
       if (inChar == '\n'){ //Command terminator encountered
            if (serialData[0] == '@' && serialData[1]=='@'){  //Command received
              char *buf = serialData+2;
              for( tok = strtok_r(buf, ",=", &p);        // split using comma
                     tok;                                          // loop while str is not null
                     tok = strtok_r(NULL, ",=", &p)                 // get subsequent tokens
                 )  
                 {
                    char isMotor = strcasecmp(tok,"Motor");
                    char isDuration = strcasecmp(tok,"Duration");
                    if (isMotor==0 || isDuration==0){
                      tok = strtok_r(NULL, ",=", &p);
                      int value = atoi(tok);  
                      if (isMotor==0 && value>=0 && value<=100){
                        state->motorOn(value, 0xFFFF);
                      } else if (isDuration==0 && value>=0)
                        state->intervalCountdown = value;
                    }   
                 }           
            } else if (serialIndex > 10 && strncasecmp("weather:", serialData, 8)==0){  //outside weather sample received
                char *buf = serialData+8;
                for( tok = strtok_r(buf, " =", &p);        // split using comma
                       tok;                                          // loop while str is not null
                       tok = strtok_r(NULL, " =", &p)                 // get subsequent tokens
                   )  
                   {
                      char isTemp = strcasecmp(tok,"temp");
                      char isRh = strcasecmp(tok,"rh");
                      if (isTemp==0 || isRh==0){
                        tok = strtok_r(NULL, " =", &p);
                        float value = atof(tok);
                        if (!isnan(value)){  
                          if (isTemp==0)
                            state->tempValues[0] = value;
                           else if (isRh==0 && value>=0)
                            state->humidityValues[0] = value;
                          }   
                      }
                   }
              }
           
            serialIndex = 0;
       }
  } 
}

void SerialInterface::reportSensors(){
      Serial.print("IAQ: ");
      printValue("Motor", (int)state->getMotor());
      if (state->intervalCountdown != 0 && state->intervalCountdown != 0xFFFF)
        printValue("Countdown", (int)state->intervalCountdown);
      printValue("AmbientLight", (int)state->ambient);
      printValue("IntermittentOn", state->intermittentOn);
      printValue("IntermittentOff", state->intermittentOff);

      
      int sensorCount = state->getSensorCount();
      for(char i=1; i<sensorCount; i++){
          float temp = state->tempValues[i];
          float humidity = state->humidityValues[i];
          
          char pos = '0'+i;
          if (!isnan(temp)){
            keyNameTemp[4] = pos;         
            printValue(keyNameTemp, temp);
          }
          if (!isnan(humidity)){
            keyNameHumidity[8] = pos;
            printValue(keyNameHumidity, humidity);
          }
      }      
      Serial.println();
}

void SerialInterface::printValue(String name, int value){
      Serial.print(name);
      Serial.print("=");
      Serial.print(value);
      Serial.print(" ");
}

void SerialInterface::printValue(String name, float value){
      Serial.print(name);
      Serial.print("=");
      if (isnan(value)){
        Serial.print("null");
      } else
      {
        Serial.print(value);
      }
      Serial.print(" ");
}


