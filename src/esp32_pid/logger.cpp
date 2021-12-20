#include "Arduino.h"
#include "logger.h"


int Logger::logLevel = LOG_LEVEL_WARN;

Logger::Logger(String className_)
{
  className = className_;
}

void Logger::log(int errorLevel, String error){
  if(errorLevel > logLevel){
    return;
  }
  logToSerial(errorLevel, className, error);

}
void Logger::log(int errorLevel, String method, String error){
  if(errorLevel > logLevel){
    return;
  }
  logToSerial(errorLevel, className + "::" + method, error);

}
void Logger::logToSerial(int errorLevel, String source, String error){
  String errorLevelStr = "";
  if(errorLevel == LOG_LEVEL_DEBUG){
    errorLevelStr = "DEBUG";
  }
  if(errorLevel == LOG_LEVEL_INFO){
    errorLevelStr = "INFO ";
  }
  if(errorLevel == LOG_LEVEL_WARN){
    errorLevelStr = "WARN ";
  }
  if(errorLevel == LOG_LEVEL_ERROR){
    errorLevelStr = "ERROR";
  }
  Serial.println(errorLevelStr + "  " + source + " - " + error);
}