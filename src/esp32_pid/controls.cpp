#include "Arduino.h"
#include "controls.h"



Controls::Controls(int upButtonPin, int downButtonPin, int shortPressMs, int longPressMs)
{
  upButton = new Button(upButtonPin, shortPressMs, longPressMs);
  downButton = new Button(downButtonPin, shortPressMs, longPressMs);
}

int Controls::getState(){
  int state = 0;
  
  state = upButton->getState();
  if(state == SHORTPRESS){
    return CONTROLACTIONUP;
  }
  if(state == LONGPRESS){
    return CONTROLACTIONMODE;
  }

  state = downButton->getState();
  if(state == SHORTPRESS){
    return CONTROLACTIONDOWN;
  }
  if(state == LONGPRESS){
    return CONTROLACTIONSELECT;
  }


  return 0;
}
