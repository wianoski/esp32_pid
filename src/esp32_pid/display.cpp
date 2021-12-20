#include "Arduino.h"
#include "display.h"


#define lineSpace 20
#define DEFAULT_BG_COLOR TFT_BLACK
#define DEFAULT_TEXT_COLOR TFT_LIGHTGREY
#define INACTIVE_MENU_ITEM_COLOR TFT_WHITE
#define SELECTED_MENU_ITEM_COLOR TFT_MAGENTA
#define ACTIVE_MENU_ITEM_COLOR TFT_ORANGE
#define CHART_COLOR_SETPOINT TFT_ORANGE
#define CHART_COLOR_OUTPUT TFT_GREEN
#define CHART_COLOR_INPUT TFT_RED
#define CHART_COLOR_BORDER TFT_DARKGREY

#define CHART_UPDATE_MS 5000

#define ADJUSTVALUE(val, toAdd, min, max) if(val+toAdd>max){val=max;} else if(val+toAdd<min){val=min;} else{val+=toAdd;}



int getIndex(int arr[], int arrSize, int value){
  for (int i = 0; i < arrSize; i++) {
    if(arr[i] == value){
      return i;
    }
  }
  return -1;
}

double getArrayMin(double a[], int length){
  double minVal = 10000;
  for(int i = 0; i < length; i++) {
      if(a[i]!= EMPTY_DATA && a[i] < minVal){
        minVal = a[i];
      }
  }
  if(minVal == 10000){
    return EMPTY_DATA;
  }
  return minVal;
}

double getArrayMax(double a[], int length){
  double maxVal = -10000;
  for(int i = 0; i < length; i++) {
      if(a[i]!= EMPTY_DATA && a[i] > maxVal){
        maxVal = a[i];
      }
  }
  if(maxVal == -10000){
    return EMPTY_DATA;
  }
  return maxVal;
}


TFT_eSPI tft = TFT_eSPI();

Display::Display(int update_ms, input_state *inputState_, double *SetPoint, double *Output, 
            double *Kp, double *Ki, double *Kd, 
            double *Kp2, double *Ki2, double *Kd2, int *pid2Band,
            bool *OperatingMode, int *SampleTime, bool *PidDirection,
            double *pOutput, double *iOutput, double *dOutput,
            int *ManualOutput, bool *resetPID, bool *saveSettings, bool *UsePrimaryPID, int *SettingAdjustmentMultiple,
            DataLogger *input, DataLogger *setpointLog, DataLogger *OutputLog)
{
  _selected_menu_item = 0; 
  inputState = inputState_;
  _SetPoint = SetPoint;
  _Output = Output;
  _Kp = Kp;
  _Ki = Ki;
  _Kd = Kd;
  _Kp2 = Kp2;
  _Ki2 = Ki2;
  _Kd2 = Kd2;
  _pid2Band = pid2Band;
  _SampleTime = SampleTime;
  _OperatingMode = OperatingMode;
  _PidDirection = PidDirection;
  _pOutput = pOutput;
  _iOutput = iOutput;
  _dOutput = dOutput;
  _ManualOutput = ManualOutput;
  _resetPID = resetPID;
  _saveSettings = saveSettings;
  _UsePrimaryPID = UsePrimaryPID;
  _SettingAdjustmentMultiple = SettingAdjustmentMultiple;
  _inputLog = input;
  _setpointLog = setpointLog;
  _outputLog = OutputLog;
  _update_ms = update_ms;
  _chart_zoom = 0;
  _select_mode = false;
  _selected_screen = 0;
  _last_update = millis();
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(DEFAULT_BG_COLOR);

}

void Display::processControlInput(int ControlInput){
  if(!ControlInput){ return; }
  else if(ControlInput == CONTROLACTIONSELECT && _selected_menu_item == SELECTEDITEM_MODE){
    *_OperatingMode = !*_OperatingMode;
    tft.fillScreen(DEFAULT_BG_COLOR);
    update_chart_area_immediately();
  }
  else if(ControlInput == CONTROLACTIONSELECT && _selected_menu_item == SELECTEDITEM_RESET){
    *_resetPID = true;
  }
  else if(ControlInput == CONTROLACTIONSELECT && _selected_menu_item == SELECTEDITEM_SAVE){
    *_saveSettings = true;
  }

  else if(ControlInput == CONTROLACTIONMODE && _select_mode){
    _select_mode = 0;
  }
  else if(ControlInput == CONTROLACTIONSELECT){
    _select_mode = !_select_mode;
  }

  // CYCLE THROUGH MENU ITEMS
  else if((ControlInput == CONTROLACTIONDOWN || ControlInput == CONTROLACTIONUP) && !_select_mode){
    int direction = 0;
    int i;
    if(ControlInput == CONTROLACTIONDOWN){direction = 1;} else{direction = -1;}
    

    if(_selected_screen==SELECTEDSCREEN_NORMAL && *_OperatingMode == AUTOMATIC){
      i = getIndex(PID_MENU_ITEMS, sizeof(PID_MENU_ITEMS)/sizeof(PID_MENU_ITEMS[0]) ,_selected_menu_item) + direction;
      int menuLen = sizeof(PID_MENU_ITEMS)/sizeof(PID_MENU_ITEMS[0]);
      if(i<0){ _selected_menu_item =  PID_MENU_ITEMS[menuLen-1];} else{_selected_menu_item = PID_MENU_ITEMS[i % (menuLen)]; }
    }
    if(_selected_screen==SELECTEDSCREEN_NORMAL && *_OperatingMode == MANUAL){
      i = getIndex(MANUAL_MENU_ITEMS, sizeof(MANUAL_MENU_ITEMS)/sizeof(MANUAL_MENU_ITEMS[0]) ,_selected_menu_item) + direction;
      int menuLen = sizeof(MANUAL_MENU_ITEMS)/sizeof(MANUAL_MENU_ITEMS[0]);
      if(i<0){ _selected_menu_item =  MANUAL_MENU_ITEMS[menuLen-1];} else{_selected_menu_item = MANUAL_MENU_ITEMS[i % (menuLen)]; }
    }
    else if(_selected_screen==SELECTEDSCREEN_LARGECHART){
      // do nothing
    }
    else if(_selected_screen==SELECTEDSCREEN_SETTINGS){
      i = getIndex(SETTINGS_MENU_ITEMS, sizeof(SETTINGS_MENU_ITEMS)/sizeof(SETTINGS_MENU_ITEMS[0]) ,_selected_menu_item) + direction;
      int menuLen = sizeof(SETTINGS_MENU_ITEMS)/sizeof(SETTINGS_MENU_ITEMS[0]);
      if(i<0){ _selected_menu_item =  SETTINGS_MENU_ITEMS[menuLen-1];} else{_selected_menu_item = SETTINGS_MENU_ITEMS[i % (menuLen)]; }
    }
  }



  // IN SELECT MODE - INCREMENT AND DECREMENT MENU ITEMS
  else if(ControlInput == CONTROLACTIONDOWN && _select_mode && _selected_screen!=SELECTEDSCREEN_LARGECHART){
    
    if(_selected_menu_item == SELECTEDITEM_SETPOINT){
      ADJUSTVALUE(*_SetPoint, -1* *_SettingAdjustmentMultiple, -100, 500);
    }
    if(_selected_menu_item == SELECTEDITEM_PID_P){
      ADJUSTVALUE(*_Kp, -1* *_SettingAdjustmentMultiple, 0, 100);
    }
    if(_selected_menu_item == SELECTEDITEM_PID_I){
      ADJUSTVALUE(*_Ki, -0.01* *_SettingAdjustmentMultiple, 0, 1);
    }
    if(_selected_menu_item == SELECTEDITEM_PID_D){
      ADJUSTVALUE(*_Kd, -10* *_SettingAdjustmentMultiple, 0, 990);
    }
    if(_selected_menu_item == SELECTEDITEM_PID2_P){
      ADJUSTVALUE(*_Kp2, -1* *_SettingAdjustmentMultiple, 0, 100);
    }
    if(_selected_menu_item == SELECTEDITEM_PID2_I){
      ADJUSTVALUE(*_Ki2, -0.01* *_SettingAdjustmentMultiple, 0, 1);
    }
    if(_selected_menu_item == SELECTEDITEM_PID2_D){
      ADJUSTVALUE(*_Kd2, -10* *_SettingAdjustmentMultiple, 0, 990);
    }
    if(_selected_menu_item == SELECTEDITEM_PID2_BAND){
      ADJUSTVALUE(*_pid2Band, -1* *_SettingAdjustmentMultiple, 0, 100);
    }
    if(_selected_menu_item == SELECTEDITEM_OUTPUT){
     ADJUSTVALUE( *_ManualOutput, -1* *_SettingAdjustmentMultiple, 0, 100);
    }
    if(_selected_menu_item == SELECTEDITEM_TIME){
      ADJUSTVALUE(*_SampleTime, -100* *_SettingAdjustmentMultiple, 0, 100000);
    }
    if(_selected_menu_item == SELECTEDITEM_DIRECTION){
      *_PidDirection = !*_PidDirection;
    }
    if(_selected_menu_item == SELECTEDITEM_SETTINGS_MULTIPLIER){
      ADJUSTVALUE(*_SettingAdjustmentMultiple, -1, 1, 20);
    }
    
  }
  else if(ControlInput == CONTROLACTIONUP && _select_mode && _selected_screen!=SELECTEDSCREEN_LARGECHART){
    if(_selected_menu_item == SELECTEDITEM_SETPOINT){
      ADJUSTVALUE(*_SetPoint, 1* *_SettingAdjustmentMultiple, -100, 500);
    }
    if(_selected_menu_item == SELECTEDITEM_PID_P){
      ADJUSTVALUE(*_Kp, +1* *_SettingAdjustmentMultiple, 0, 100);
    }
    if(_selected_menu_item == SELECTEDITEM_PID_I){
      ADJUSTVALUE(*_Ki, +0.01* *_SettingAdjustmentMultiple, 0, 1);
    }
    if(_selected_menu_item == SELECTEDITEM_PID_D){
      ADJUSTVALUE(*_Kd, +10* *_SettingAdjustmentMultiple, 0, 990);
    }
    if(_selected_menu_item == SELECTEDITEM_PID2_P){
      ADJUSTVALUE(*_Kp2, +1* *_SettingAdjustmentMultiple, 0, 100);
    }
    if(_selected_menu_item == SELECTEDITEM_PID2_I){
      ADJUSTVALUE(*_Ki2, +0.01* *_SettingAdjustmentMultiple, 0, 1);
    }
    if(_selected_menu_item == SELECTEDITEM_PID2_D){
      ADJUSTVALUE(*_Kd2, +10* *_SettingAdjustmentMultiple, 0, 990);
    }
    if(_selected_menu_item == SELECTEDITEM_PID2_BAND){
      ADJUSTVALUE(*_pid2Band, +1* *_SettingAdjustmentMultiple, 0, 100);
    }
    if(_selected_menu_item == SELECTEDITEM_OUTPUT){
     ADJUSTVALUE(*_ManualOutput, +1* *_SettingAdjustmentMultiple, 0, 100);
    }
    if(_selected_menu_item == SELECTEDITEM_TIME){
      ADJUSTVALUE(*_SampleTime, +100* *_SettingAdjustmentMultiple, 0, 100000);
    }
    if(_selected_menu_item == SELECTEDITEM_DIRECTION){
      *_PidDirection = !*_PidDirection;
    }
    if(_selected_menu_item == SELECTEDITEM_SETTINGS_MULTIPLIER){
      ADJUSTVALUE(*_SettingAdjustmentMultiple, +1, 1, 20);
    }

  }





  // LONG PRESS MODE BUTTON - CYCLE THROUGH SCREENS
  else if(ControlInput == CONTROLACTIONMODE){
    _selected_screen = (_selected_screen+1) % 3;


    if(_selected_screen==SELECTEDSCREEN_NORMAL){
      _selected_menu_item = PID_MENU_ITEMS[0];
      

    }

    else if(_selected_screen==SELECTEDSCREEN_LARGECHART){

    }

    else if(_selected_screen==SELECTEDSCREEN_SETTINGS){
      _selected_menu_item = SETTINGS_MENU_ITEMS[0];

    }

    tft.fillScreen(DEFAULT_BG_COLOR);
    update_chart_area_immediately();

    
  }

  // // CHART ZOOM AREA
  // else if(ControlInput == CONTROLACTIONUP && _selected_screen==SELECTEDSCREEN_LARGECHART){
  //   _chart_zoom = _chart_zoom - 1;
  //   update_chart_area_immediately();
  // }
  // else if(ControlInput == CONTROLACTIONDOWN && _selected_screen==SELECTEDSCREEN_LARGECHART){
  //   _chart_zoom = _chart_zoom + 1;
  //   update_chart_area_immediately();
  // }
  

  update_non_chart_area_immediately();
}

void Display::update_non_chart_area_immediately(){
  
  if(_selected_screen==SELECTEDSCREEN_NORMAL){
    if(*_OperatingMode == AUTOMATIC){
      this->draw_mode_section();
      this->draw_setpoint_section();
      this->draw_pid_section();
    }else{
      this->draw_mode_section();
      this->draw_setpoint_section();
      this->draw_manual_output_section();
    }
    
  }

  if(_selected_screen==SELECTEDSCREEN_LARGECHART){
    this->draw_mode_section();
    // this->draw_setpoint_section();
  }

  if(_selected_screen==SELECTEDSCREEN_SETTINGS){
    this->draw_settings_section();
  }

  _last_update = millis();
}

void Display::update_chart_area_immediately(){
  if(_selected_screen==SELECTEDSCREEN_NORMAL){
    this->draw_chart_section();
    _last_chart_update = millis();
    
  }

  if(_selected_screen==SELECTEDSCREEN_LARGECHART){
    this->draw_chart_section();
    _last_chart_update = millis();
  }

  if(_selected_screen==SELECTEDSCREEN_SETTINGS){
    // this->draw_mode_section();
  }
  
}

void Display::update_(){
  if(millis()-_last_update > _update_ms ){
    update_non_chart_area_immediately();
  }
  if(millis()- _last_chart_update > CHART_UPDATE_MS){
    update_chart_area_immediately();
  }
}

void Display::_draw_menu_item(String text, int x, int y, int menuItem){
  if(_selected_menu_item == menuItem){
      if(_select_mode){_draw_text(text, x, y, 2, SELECTED_MENU_ITEM_COLOR, DEFAULT_BG_COLOR);}
      else{ _draw_text(text, x, y, 2, ACTIVE_MENU_ITEM_COLOR, DEFAULT_BG_COLOR); }
    }
  else{
    _draw_text(text, x, y, 2, INACTIVE_MENU_ITEM_COLOR, DEFAULT_BG_COLOR);
  }
}

void Display::_draw_text(String text, int x, int y, int textSize = 2, int textColorFG = DEFAULT_TEXT_COLOR, int textColorBG = DEFAULT_BG_COLOR){
  tft.setTextSize(textSize);
  tft.setTextColor(textColorFG, textColorBG);   
  tft.drawString( text,x,y);
}

void Display::draw_setpoint_section(){
  _draw_text("Set ",0, lineSpace*1);
  if(inputState->error){ _draw_text( "ERROR ", 80, lineSpace*1); }
  else{ _draw_text( String(inputState->value) + " ", 80, lineSpace*1); }

  if(inputState->useRedundantSensor){
    if(isnan(inputState->input1)){
      _draw_text( _rightPadString("Err",4," "), 50, lineSpace*1, 1);
    }
    else{
      _draw_text( _rightPadString(String(int(round(inputState->input1))),4," "), 50, lineSpace*1, 1);
    }
    
    if(isnan(inputState->input2)){
       _draw_text( _rightPadString("Err",4," "), 50, lineSpace + 8, 1);
    }
    else{
      _draw_text( _rightPadString(String(int(round(inputState->input2))),4," "), 50, lineSpace + 8, 1); 
    }
     
  }
  if(*_OperatingMode == AUTOMATIC){
    _draw_text( String("/"), 170, lineSpace*1);
    _draw_menu_item(String(int(*_SetPoint))+ "   ", 190, lineSpace*1, SELECTEDITEM_SETPOINT);
  }
}

void Display::draw_mode_section(){ 
  if(_selected_screen==SELECTEDSCREEN_NORMAL){
    if(*_OperatingMode == AUTOMATIC){
      _draw_text( "Mode", 0, 0); 
      _draw_menu_item("PID", 80, 0, SELECTEDITEM_MODE);
    }else{
      _draw_text( "Mode", 0, 0); 
      _draw_menu_item("Manual", 80, 0, SELECTEDITEM_MODE);
    }
  }

  else if(_selected_screen==SELECTEDSCREEN_SETTINGS){_draw_text( "Mode", 0, 0); _draw_menu_item("Settings", 80, 0, SELECTEDITEM_MODE);}
  else if(_selected_screen==SELECTEDSCREEN_LARGECHART){}
}

void Display::draw_pid_section(){
  //   Control Values
  _draw_text( "Gain   ", 0, lineSpace*2);
  _draw_menu_item( _rightPadString(String(int(*_Kp)),3, " ") , 80, lineSpace*2,SELECTEDITEM_PID_P ); 
  _draw_menu_item( _rightPadString(String(int(*_Ki*100)),3," "), 120, lineSpace*2, SELECTEDITEM_PID_I ); 
  _draw_menu_item( _rightPadString(String(int(*_Kd)),3," "), 160, lineSpace*2, SELECTEDITEM_PID_D );
  if(*_UsePrimaryPID){
    _draw_text( "Normal    " , 125, 0, 1); 
  }
  else{
    _draw_text( "Aggressive" , 125, 0, 1); 
  }
  
  _draw_text( String(int(round(*_pOutput))) + " " + String(int(round(*_iOutput))) + " "+ String(int(round(*_dOutput))) + "       ", 125, 8, 1); 
  _draw_text(String(int(round(*_Output))) + "%      ", 190, 0, 2); 
}

void Display::draw_settings_section(){
  _draw_text( "ADVANCED SETTINGS", 0, 0, 2, DEFAULT_TEXT_COLOR); 

  _draw_text("Time ", 0, lineSpace*1); 
  _draw_menu_item(_rightPadString(String(*_SampleTime),5," "), 80, lineSpace*1, SELECTEDITEM_TIME ); 

  _draw_text("Dir ", 0, lineSpace*2); 
  if( *_PidDirection){ _draw_menu_item("Reverse  ", 80, lineSpace*2, SELECTEDITEM_DIRECTION ); }
  else{ _draw_menu_item("Direct  ", 80, lineSpace*2, SELECTEDITEM_DIRECTION ); }


  _draw_text( "aPID", 0, lineSpace*3);
  _draw_menu_item( _rightPadString(String(int(*_Kp2)),3, " ") , 80, lineSpace*3,SELECTEDITEM_PID2_P ); 
  _draw_menu_item( _rightPadString(String(int(*_Ki2*100)),3," "), 120, lineSpace*3, SELECTEDITEM_PID2_I ); 
  _draw_menu_item( _rightPadString(String(int(*_Kd2)),3," "), 160, lineSpace*3, SELECTEDITEM_PID2_D );

  _draw_text( "aBand", 0, lineSpace*4);
  _draw_menu_item( _rightPadString(String(int(*_pid2Band)),3," "), 80, lineSpace*4, SELECTEDITEM_PID2_BAND );

  _draw_text( "Mult", 0, lineSpace*5);
  _draw_menu_item( _rightPadString(String(int(*_SettingAdjustmentMultiple)),3," "), 80, lineSpace*5, SELECTEDITEM_SETTINGS_MULTIPLIER );

  

  _draw_text("Addl ", 0, lineSpace*6); 

  if( *_resetPID){ _draw_menu_item("..... ", 80, lineSpace*6, SELECTEDITEM_RESET ); }
  else{ _draw_menu_item("Reset", 80, lineSpace*6, SELECTEDITEM_RESET ); }
 
  if( *_saveSettings){ _draw_menu_item("....", 160, lineSpace*6, SELECTEDITEM_SAVE ); }
  else{ _draw_menu_item("Save", 160, lineSpace*6, SELECTEDITEM_SAVE ); }
}

void Display::draw_manual_output_section(){
  _draw_text( "Out", 0, lineSpace*2); 
  _draw_menu_item(_rightPadString(String(*_ManualOutput),3," "), 80, lineSpace*2, SELECTEDITEM_OUTPUT );
}

void Display::draw_line_chart(int topPixel, int bottomPixel, int leftPixel, double minY, double maxY, int color, double data[], int dataLen, int lastPointLen=0){
  // DRAW CHART DATA
  double current_scaled_y;
  double current_pixel_y;
  double last_pixel_y;
  bool start_line = false;
  double dataRangeWidth = maxY - minY;
  double yRangeWidth = bottomPixel - topPixel;

  for(int i = 0; i < dataLen; i++) {
    if(data[i] != EMPTY_DATA){

      // SCALE DATA
      current_scaled_y =  (data[i] - minY) / dataRangeWidth * yRangeWidth;
      //                  [  PERCENTAGE OF DATA ALONG RANGE  ]
      //                  [  VALUE OF SCALED DATA IN TERMS OF SCALED RANGE ]

      current_pixel_y = bottomPixel - current_scaled_y;

      // DRAW PIXEL ONLY IF INSIDE CHART AREA
      if(current_pixel_y <= bottomPixel && current_pixel_y >= topPixel && start_line){

        // tft.drawPixel(i+leftPixel, current_pixel_y, color);
        tft.drawLine(i+leftPixel-1, last_pixel_y, i+leftPixel, current_pixel_y, color);
        // Serial.println(bottomPixel);
        // Serial.println(current_scaled_y);
        // Serial.println(current_pixel_y);
        // Serial.println(last_pixel_y);
      }


      last_pixel_y = current_pixel_y;
      start_line = true;
    }
  }

  if(lastPointLen>0){
    tft.drawLine(dataLen, last_pixel_y, dataLen+lastPointLen, last_pixel_y, color);
  }
}




void Display::draw_chart_section(){
  // CHART PARAMS
  double setpoint = _setpointLog->getItem(DATA_ARRAY_SIZE-1);
  double current_input = _inputLog->getItem(DATA_ARRAY_SIZE-1);
  int top_chart_bound;
  int bottom_chart_bound;
  if(_selected_screen==SELECTEDSCREEN_LARGECHART){top_chart_bound = 10;bottom_chart_bound = 130;} else{top_chart_bound = 60;bottom_chart_bound = 135;}
  

  // CHART MIN/MAX PADDING
  double max_y = max(_inputLog->max_(),_setpointLog->max_());
  double min_y = min(_inputLog->min_(),_setpointLog->min_());
  max_y += (max_y - min_y) * 0.15;
  min_y -= (max_y - min_y) * 0.15;


  //CHART BACKGROUND - CLEAR PRIOR CHART DRAWING
  tft.fillRect(0, top_chart_bound, 240, 240, DEFAULT_BG_COLOR);



  ////////////////////////////////////////////
  ///////       CHART GRIDLINES       ////////
  ////////////////////////////////////////////


  double chartMajorGridLine = 100;
  double rangeDelta = max_y - min_y;
  if(rangeDelta<=5 ){ chartMajorGridLine = 1;}
  else if(rangeDelta<=20 ){ chartMajorGridLine = 5;}
  else if(rangeDelta<=40 ){ chartMajorGridLine = 10;}
  else if(rangeDelta<=80 ){ chartMajorGridLine = 20;}
  else if(rangeDelta<=125 ){ chartMajorGridLine = 25;}
  else if(rangeDelta<=250){ chartMajorGridLine = 50;}

  // readjust min and max to fall on gridline
  min_y = int(min_y/chartMajorGridLine)*chartMajorGridLine;
  max_y = (int(max_y/chartMajorGridLine)+1)*chartMajorGridLine;

  //draw gridlines
  double current_line = min_y + chartMajorGridLine;
  while(current_line<max_y){
    for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      // DRAW DOTTED LINE
      if(i%3==0){ tft.drawPixel(i, int(bottom_chart_bound - (current_line - min_y)/(max_y - min_y)*(bottom_chart_bound-top_chart_bound)) , TFT_DARKGREY);}
    }
    current_line += chartMajorGridLine;
  }


  
  ////////////////////////////////////////////
  ///////          LINE CHARTS        ////////
  ////////////////////////////////////////////

  double input_range_width = max_y - min_y;
  double setpoint_percent = (setpoint-min_y)/input_range_width;
  double current_input_percent = (current_input-min_y)/input_range_width;
  int text_height = 8;
  int chart_range_width = bottom_chart_bound-top_chart_bound;
  double currentInputLabelPixel =  bottom_chart_bound  - (current_input_percent*chart_range_width) - text_height/2.0;
  double setpointLabelPixel = bottom_chart_bound - (setpoint_percent*chart_range_width) - text_height/2.0;
  double currentOutput = (*_OperatingMode == AUTOMATIC) ? *_Output : *_ManualOutput;
  double currentOutputLinePixel = bottom_chart_bound - 2  - (currentOutput/100.0 * (chart_range_width-3));
  double currentOutputLabelPixel = currentOutputLinePixel - text_height/2.0;
  int topLabelBound = top_chart_bound + text_height + 1;
  int bottomLabelBound = bottom_chart_bound - text_height*2 - 1;
  int maxLabelPos;
  int minLabelPos;
  bool labelOverlap = false;


  tft.drawRect(0, top_chart_bound, DATA_ARRAY_SIZE+2,  bottom_chart_bound-top_chart_bound, CHART_COLOR_BORDER);

  if(currentOutputLinePixel<bottom_chart_bound && currentOutputLinePixel>top_chart_bound){
    for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
        // DRAW DOTTED CURRENT OUTPUT LINE
      if((i+3)%6==0){ tft.drawPixel(i, currentOutputLinePixel, TFT_DARKGREEN);}
    }
  }
  

  draw_line_chart(top_chart_bound+1, bottom_chart_bound-2, 1, min_y, max_y, CHART_COLOR_SETPOINT, _setpointLog->getDataArray(), DATA_ARRAY_SIZE, 3);
  draw_line_chart(top_chart_bound+1, bottom_chart_bound-2, 1, 0, 100, CHART_COLOR_OUTPUT, _outputLog->getDataArray(), DATA_ARRAY_SIZE, 3);
  draw_line_chart(top_chart_bound+1, bottom_chart_bound-2, 1, min_y, max_y, CHART_COLOR_INPUT, _inputLog->getDataArray(), DATA_ARRAY_SIZE, 3);
  

  


  ////////////////////////////////////////////
  ///////      CHART DATA LABELS      ////////
  ////////////////////////////////////////////


  #define CHART_DATA_LABEL_X DATA_ARRAY_SIZE+8

  if(_inputLog->max_() != EMPTY_DATA && _setpointLog->max_() != EMPTY_DATA){
    

    // DON"T LET SETPOINT AND CURRENT INPUT OVERLAP
    if(abs(currentInputLabelPixel-setpointLabelPixel)<text_height+4){
      if(current_input > setpoint){
        currentInputLabelPixel = bottom_chart_bound - ((current_input_percent + setpoint_percent)/2 * chart_range_width) - text_height - 1;
        setpointLabelPixel = bottom_chart_bound - ((current_input_percent + setpoint_percent)/2 * chart_range_width) + 1;
        labelOverlap = true;
      }else{
        setpointLabelPixel = bottom_chart_bound - ((current_input_percent + setpoint_percent)/2 * chart_range_width) - text_height - 1;
        currentInputLabelPixel = bottom_chart_bound - ((current_input_percent + setpoint_percent)/2 * chart_range_width) + 1;
        labelOverlap = true;
      }
    }
    
    // PREVENT SETPOINT AND CURRENT INPUT LABELS TO DISPLAY OVER MIN AND MAX

    if(currentInputLabelPixel < topLabelBound && labelOverlap){ currentInputLabelPixel = topLabelBound; setpointLabelPixel = topLabelBound + text_height + 2; }
    if(currentInputLabelPixel > bottomLabelBound && labelOverlap){ currentInputLabelPixel = bottomLabelBound; setpointLabelPixel = bottomLabelBound - text_height - 2; }
    if(setpointLabelPixel < topLabelBound && labelOverlap){ setpointLabelPixel = topLabelBound; currentInputLabelPixel =  topLabelBound + text_height + 2;}
    if(setpointLabelPixel > bottomLabelBound && labelOverlap){ setpointLabelPixel = bottomLabelBound; currentInputLabelPixel =  bottomLabelBound - text_height - 2; }

    if(currentInputLabelPixel < topLabelBound && !labelOverlap){ currentInputLabelPixel = topLabelBound; }
    if(currentInputLabelPixel > bottomLabelBound && !labelOverlap){ currentInputLabelPixel = bottomLabelBound; }
    if(setpointLabelPixel < topLabelBound && !labelOverlap){ setpointLabelPixel = topLabelBound; }
    if(setpointLabelPixel > bottomLabelBound && !labelOverlap){ setpointLabelPixel = bottomLabelBound; }


    if(_selected_screen==SELECTEDSCREEN_LARGECHART){
      maxLabelPos = bottom_chart_bound-chart_range_width - text_height/2;
      minLabelPos =  bottom_chart_bound-text_height/2;
      // SMALL TEXT ABOVE LARGE CHART

      if(*_OperatingMode == AUTOMATIC){
        if(*_UsePrimaryPID){ _draw_text( "    Normal" , 140, 0, 1); } else{ _draw_text( "Aggressive" , 140, 0, 1); }
        _draw_text( String(int(round(*_Output))) + "%      ", 0, 0, 1 , DEFAULT_TEXT_COLOR, DEFAULT_BG_COLOR);
        _draw_text( "                ", 30 , 0, 1);
        _draw_text( "(" + String(int(round(*_pOutput))) + " " + String(int(round(*_iOutput))) + " "+ String(int(round(*_dOutput))) + ")", 30 , 0, 1); 
      }else{      
        _draw_text( "    Manual" , 140, 0, 1);
        _draw_text( String(*_ManualOutput) + "%      ", 0, 0, 1 , DEFAULT_TEXT_COLOR, DEFAULT_BG_COLOR);
        _draw_text( "                ", 30 , 0, 1);
      }
      
    }
    else{
      maxLabelPos = bottom_chart_bound-chart_range_width;
      minLabelPos =  bottom_chart_bound-text_height;
      // CURENT OUTPUT
       // _draw_text( String(int(round(*_Output))) + "%", 3, minLabelPos - 2 , 1 , CHART_COLOR_OUTPUT, DEFAULT_BG_COLOR); 

    }

    if(currentOutputLabelPixel<top_chart_bound+text_height){currentOutputLabelPixel = top_chart_bound + 3;}
    if(currentOutputLabelPixel>bottom_chart_bound-text_height){currentOutputLabelPixel = bottom_chart_bound-text_height - 2;}
    _draw_text( String(int(round(currentOutput))) + "%", 3, currentOutputLabelPixel , 1 , CHART_COLOR_OUTPUT, DEFAULT_BG_COLOR);

     

    // MIN/MAX 
    _draw_text( String(int(max_y)), CHART_DATA_LABEL_X, maxLabelPos, 1 , TFT_LIGHTGREY, DEFAULT_BG_COLOR);
    _draw_text( String(int(min_y)), CHART_DATA_LABEL_X, minLabelPos, 1 , TFT_LIGHTGREY, DEFAULT_BG_COLOR);
    // SETPOINT CURRENT INPUT
    _draw_text( String(int(setpoint)), CHART_DATA_LABEL_X, setpointLabelPixel, 1 , CHART_COLOR_SETPOINT, DEFAULT_BG_COLOR);
    _draw_text( String(current_input), CHART_DATA_LABEL_X, currentInputLabelPixel, 1 , CHART_COLOR_INPUT, DEFAULT_BG_COLOR);

  }
  


  // if(_chart_zoom && _selected_screen==SELECTEDSCREEN_LARGECHART){
  //   _draw_text( "Zoom: " + String(-_chart_zoom) + " ("+ String(pow(2,abs(_chart_zoom)-1) / 10.0) +")", 100, bottom_chart_bound-10, 1 , TFT_LIGHTGREY, DEFAULT_BG_COLOR);

  // }
  
}

String Display::_rightPadString(String text, int strLength, String padVal){
  
  String output = text;
  if(text.length()>=strLength){
    return output;
  }
  for (int i = 0; i < strLength-text.length(); i++) {
    output += padVal;
  }
  return output;
}

String Display::_convertNumberToStr(float value, int precision){
  if (precision<=0){
    return String(int(round(value)));
  }
  
  String out = "";
  int multiplier = pow(10, precision);
  int whole = int(round(value*multiplier)/multiplier);
  int remainder = round(value*multiplier)/multiplier - whole;
  out += String(whole);
  out += ".";
  
  //Need to force decimal points if remainder is 0
  if(remainder==0){
    for (int i = 0; i < precision; i++) {
      out += "0";
    }
  }
  else{
    out += String(int(remainder * multiplier));
  }
  return out;
}

