#ifndef Controls_
#define Controls_

#include "Arduino.h"
#include "button.h"

#define CONTROLACTIONUP 1
#define CONTROLACTIONDOWN 2
#define CONTROLACTIONSELECT 3
#define CONTROLACTIONMODE 4

class Controls{
  public:
    Controls(int upButtonPin, int downButtonPin, int shortPressMs, int longPressMs);
    int getState();
  private:
    Button *upButton;
    Button *downButton;
    Button *incrementButton;
    Button *decrementButton;
    int button_count;
};

#endif
