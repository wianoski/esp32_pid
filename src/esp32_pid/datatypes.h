#ifndef MydataTypes
#define MydataTypes

struct esp32_pid_settings{
  // INITIAL SETTINGS PARAMETERS
  double SetPoint = 80;
  bool PidDirection = 0; // DIRECT
  bool OperatingMode = 1; //AUTOMATIC
  int SampleTime = 1000;
  double Kp=5;
  double Ki=0;
  double Kd=0;
  double Kp2=10;
  double Ki2=0;
  double Kd2=0;
  int pid2Band = 10;
  int dataLogDelay = 10000; // 10 seconds - gives about 40 minutes of chart
  int SettingAdjustmentMultiple = 1;
  unsigned int maxSensorQueryTime = 10000; // max amount of time in MS to spend trying to get valid result from each sensor
};

struct last_values{
    double Kp, Ki, Kd;
    double Kp2, Ki2, Kd2;
    bool OperatingMode;
    bool PidDirection;
    int SampleTime;
    bool UsePrimaryPID = true;
    double myOutput=0;
};


struct output_window_values{
    bool use = false;
    unsigned long windowLengthMs;
    unsigned long cycleStartTime;
    unsigned long cycleTimeElapsed;
    unsigned long cycleOnTime = 0;
};

struct pid_state_values{
    double PidOutputSum = 0;
    double PidOutputP = 0;
    double PidOutputI = 0;
    double PidOutputD = 0;
    bool UsePrimaryPID = true;
    int ManualOutput = 0;
};


struct input_state{
  double value;
  double input1;
  double input2;
  int useRedundantSensor;
  int error = 0;
  unsigned int last_update;
  bool useAverage;
};


#endif