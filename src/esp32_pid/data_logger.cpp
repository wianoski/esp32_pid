
#include "Arduino.h"
#include "data_logger.h"


DataLogger::DataLogger(int log_delay_ms){
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      _data_array[i] = EMPTY_DATA;
    }
  _log_delay_ms = log_delay_ms;
  _current_pos = 0;
  _last_logged = millis();
}

bool DataLogger::logData(double log_data){
  if(millis() - _last_logged > _log_delay_ms){
    if(isnan(log_data)){
      _data_array[_current_pos] = ERROR_VALUE;
    }
    else{
      _data_array[_current_pos] = log_data;
    }
    
    if(_current_pos==DATA_ARRAY_SIZE-1){
      _current_pos = 0;
    }
    else{_current_pos += 1;}

    for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      getItem(i);
    }
   
    _last_logged = millis();
    
    return true;
  }
  return false;
}

double DataLogger::getItem(int index){
//  Serial.print( (_current_pos+index)%DATA_ARRAY_SIZE ); Serial.print(",");
//  Serial.print(_current_pos); Serial.print(",");
//  Serial.print(index); Serial.print(",");
//  Serial.print(_data_array[(_current_pos+index)%DATA_ARRAY_SIZE]); Serial.print("\n");
//  
  return _data_array[(_current_pos+index)%DATA_ARRAY_SIZE];
}

double * DataLogger::getDataArray(){
  static double data[DATA_ARRAY_SIZE];
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      data[i] = getItem(i);
    }
  return data;
}

double DataLogger::min_(){
  double minVal = 10000;
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      if(_data_array[i]!= EMPTY_DATA && _data_array[i] < minVal){
        minVal = _data_array[i];
      }
  }
  if(minVal == 10000){
    return EMPTY_DATA;
  }
  return minVal;
}

double DataLogger::max_(){
  double maxVal = -10000;
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      if(_data_array[i]!= EMPTY_DATA && _data_array[i] > maxVal){
        maxVal = _data_array[i];
      }
  }
  if(maxVal == -10000){
    return EMPTY_DATA;
  }
  return maxVal;
}




