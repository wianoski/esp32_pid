#include "Arduino.h"
#include "button.h"

#define DELAYAFTERLONGPRESS 250

Button::Button(int pin, int short_press_ms, int long_press_ms)
{
  _pinNumber = pin;
  _short_press_ms = short_press_ms;
  _long_press_ms = long_press_ms;
  _press_start_time = 0;
  pinMode(pin, INPUT);
}


int Button::getState()
{
  if (digitalRead(_pinNumber)==0)
  {
    // Button went from not pressed to pressed
    if(_press_start_time==0 || millis()-_last_long_press<DELAYAFTERLONGPRESS){
      _press_start_time = millis();
    }

    // Long Press
    if(millis()-_press_start_time>=_long_press_ms){
      _press_start_time = 0;
      _last_long_press = millis();
      return LONGPRESS;
    }

    // Button still pressed, do nothing and wait for release
    return 0;
  }
  else{
    
     // button went from pressed to not pressed
    if(_press_start_time>0){
      
      // Long Press
      if(millis()-_press_start_time>=_long_press_ms){
        _press_start_time = 0;
        _last_long_press = millis();
        return LONGPRESS;
      }

      // Short Press
      if(millis()-_press_start_time>=_short_press_ms){
        _press_start_time = 0;
        return SHORTPRESS;
      }
    }

    // Press or glitch shorter than short press threshold, reset
    _press_start_time = 0;
    return 0;
    
  }

}
