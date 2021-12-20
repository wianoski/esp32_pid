#include "Arduino.h"
#include "input_.h"


Input_::Input_(double (*readInputFunction_)())
{
  readInputA = readInputFunction_;
  logger = new Logger("Input_");
}

input_state Input_::read()
{
  input_state myState;
  myState.useRedundantSensor = redundantInputEnabled;
  myState.useAverage = redundantUseAverage;
  double InputA = queryInput(readInputA, maxSensorQueryTime, 1);


  // Redundant Sensor
  if(redundantInputEnabled){
    double InputB = queryInput(readInputB, maxSensorQueryTime, 2);

    // Check failsafe
    if(isnan(InputA) || isnan(InputB)){
      if( isnan(InputA) ){
        myState.error = SENSOR1_ERROR;
      }
      if( isnan(InputB) ){
        myState.error = SENSOR2_ERROR;
      }
    }
    else if(abs(InputA-InputB) > redundantMaxDifference){
      myState.error = REDUNDANT_SENSOR_MISMATCH;
    }
    else{
      myState.error = 0;
    }
   
    if(myState.error){
      myState.value = nan("");
    }
    else if(redundantUseAverage){
      myState.value = (InputA+InputB)/2.0;
    }
    else{
      myState.value = InputA;
    }


    switch(myState.error) {
      case SENSOR1_ERROR:
        logger->log(LOG_LEVEL_ERROR, "read","Sensor 1 error." );
        break;
      case SENSOR2_ERROR:
        logger->log(LOG_LEVEL_ERROR, "read","Sensor 2 error." );
        break;
      case REDUNDANT_SENSOR_MISMATCH:
        if(lastReadingA != InputA && lastReadingB != InputB){
          logger->log(LOG_LEVEL_ERROR, "read", "REDUNDANT SENSOR DISAGREEMENT! Sensor 1: " + String(InputA) + ", Sensor 2: " + String(InputB) + ". Max allowed difference " + String(redundantMaxDifference) );
        } 
        break;
      default:
        break;
    }

    
    myState.input1 = InputA;
    myState.input2 = InputB;
    lastReadingA = InputA;
    lastReadingB = InputB;
    myState.last_update = millis();
    return myState;
  }

  // Failsafe
  if( isnan(InputA) ){
    myState.error = SENSOR1_ERROR;
    logger->log(LOG_LEVEL_WARN, "read","Sensor error." );
  }
  else{
    myState.value = InputA;
    myState.error = 0;
  }
  lastReadingA = InputA;
  myState.last_update = millis();
  return myState;
}

void Input_::useRedundantInput(double (*readInputFunction_)(), int maxDifference, bool useAverage){
  readInputB = readInputFunction_;
  redundantMaxDifference = maxDifference;
  redundantUseAverage = useAverage;
  redundantInputEnabled = true;
}


double Input_::queryInput(double (*readInputFunction_)(), int maxQueryTimeMS, int sensorId){
  unsigned long queryStartTime = millis();
  double result;
  int loopCount = 0;
  while(millis()-queryStartTime<maxQueryTimeMS){
     result = (*readInputFunction_)();
     if(!isnan(result)){
      // if(loopCount>0){
      //   Serial.println("Sensor " +String(sensorId) + " took " + String(millis()-queryStartTime) + " ms and " + String(loopCount+1) + " tries.  Max query time " + String(maxQueryTimeMS) + " ms.");
      // }
      return result;
     }
     delay(loopCount*100);
     loopCount +=1;
  }
  return nan("");
}
