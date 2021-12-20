#ifndef TftDisplay
#define TftDisplay

#include "Arduino.h"
#include "controls.h"
#include "pid.h"
#include "data_logger.h"
#include "datatypes.h"
#include <TFT_eSPI.h>
#include "input_.h"

#define SELECTEDITEM_MODE 0
#define SELECTEDITEM_SETPOINT 1
#define SELECTEDITEM_PID_P 2
#define SELECTEDITEM_PID_I 3
#define SELECTEDITEM_PID_D 4
#define SELECTEDITEM_OUTPUT 5
#define SELECTEDITEM_TIME 6
#define SELECTEDITEM_DIRECTION 7
#define SELECTEDITEM_PID2_P 8
#define SELECTEDITEM_PID2_I 9
#define SELECTEDITEM_PID2_D 10
#define SELECTEDITEM_PID2_BAND 11
#define SELECTEDITEM_SETTINGS_MULTIPLIER 12
#define SELECTEDITEM_RESET 13
#define SELECTEDITEM_SAVE 14

#define SELECTEDSCREEN_NORMAL 0
#define SELECTEDSCREEN_LARGECHART 1
#define SELECTEDSCREEN_SETTINGS 2
    

class Display
{
  public:
    Display(int update_ms, input_state *inputState_, double *SetPoint, double *Output, 
            double *Kp, double *Ki, double *Kd, 
            double *Kp2, double *Ki2, double *Kd2, int *pid2Band,
            bool *OperatingMode, int *SampleTime, bool *PidDirection,
            double *pOutput, double *iOutput, double *dOutput,
            int *ManualOutput, bool *resetPID, bool *saveSettings, bool *UsePrimaryPID, int *SettingAdjustmentMultiple,
            DataLogger *inputLog, DataLogger *setpointLog, DataLogger *outputLog);
    void processControlInput(int ControlInput);
    void update_();
  private:
    int _selected_menu_item;
    unsigned long _last_update;
    unsigned long _last_chart_update;
    unsigned int _update_ms;
    input_state *inputState;
    double *_SetPoint;
    double *_Output;
    double *_Kp;
    double *_Ki;
    double *_Kd;
    double *_Kp2;
    double *_Ki2;
    double *_Kd2;
    int *_pid2Band;
    int *_SampleTime;
    bool *_OperatingMode;
    bool *_PidDirection;
    double *_pOutput;
    double *_iOutput;
    double *_dOutput;
    int *_ManualOutput;
    bool *_resetPID;
    bool *_saveSettings;
    bool *_UsePrimaryPID;
    int *_SettingAdjustmentMultiple;
    bool _select_mode;
    int _chart_zoom;
    int _selected_screen;
    DataLogger *_inputLog;
    DataLogger *_setpointLog;
    DataLogger *_outputLog;
    void draw_setpoint_section();
    void draw_mode_section();
    void draw_pid_section();
    void draw_chart_section();
    void draw_manual_output_section();
    void draw_settings_section();
    void update_non_chart_area_immediately();
    void update_chart_area_immediately();
    void update_immediately();
    void _draw_menu_item(String text, int x, int y, int menuItem);
    void _draw_text(String text, int x, int y, int textSize, int textColorFG, int textColorBG);
    void draw_line_chart(int topPixel, int bottomPixel, int leftPixel, double minY, double maxY, int color, double data[], int dataLen, int lastPointLen);
    String _convertNumberToStr(float value, int precision);
    String _rightPadString(String text, int strLength, String padVal);
    int PID_MENU_ITEMS[5] = {SELECTEDITEM_MODE, SELECTEDITEM_SETPOINT, SELECTEDITEM_PID_P, SELECTEDITEM_PID_I, SELECTEDITEM_PID_D};
    int MANUAL_MENU_ITEMS[2] = {SELECTEDITEM_MODE, SELECTEDITEM_OUTPUT}; 
    int SETTINGS_MENU_ITEMS[9] = { SELECTEDITEM_TIME, SELECTEDITEM_DIRECTION, SELECTEDITEM_PID2_P, SELECTEDITEM_PID2_I, SELECTEDITEM_PID2_D, SELECTEDITEM_PID2_BAND, SELECTEDITEM_SETTINGS_MULTIPLIER, SELECTEDITEM_RESET, SELECTEDITEM_SAVE}; 
};

#endif
