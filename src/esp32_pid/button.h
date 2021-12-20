#ifndef Button_
#define Button_

#include "Arduino.h"

class Button{
  public:
     //Constants used in some of the functions below
    #define SHORTPRESS 1
    #define LONGPRESS  2
    
    Button(int pin, int short_press_ms, int long_press_ms);
    int getState();
  private:
    int _pinNumber;
    unsigned int _short_press_ms;
    unsigned int _long_press_ms;
    unsigned long _press_start_time;
    unsigned long _last_long_press;
};

#endif
