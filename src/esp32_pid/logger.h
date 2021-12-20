#ifndef Logger_
#define Logger_

// #include "Arduino.h"
enum LOG_LEVELS { LOG_LEVEL_OFF, LOG_LEVEL_INFO, LOG_LEVEL_WARN, LOG_LEVEL_DEBUG, LOG_LEVEL_ERROR };

class Logger{
  
  public:

     //Constants used in some of the functions below
    // #define SHORTPRESS 1
    // #define LONGPRESS  2
    static int logLevel;
    Logger(String className_);
    void log(int errorLevel, String error);
    void log(int errorLevel, String method, String error);

  private:
    String className;
    void logToSerial(int errorLevel, String source, String error);

};


#endif
