#ifndef SettingsStore_
#define SettingsStore_

#include "Arduino.h"


class SettingsStore{
  public:
    SettingsStore(double *SetPoint, double *Kp, double *Ki, double *Kd, double *Kp2, double *Ki2, double *Kd2, bool *PidDirection, int *SampleTime, int *pid2Band, int *SettingAdjustmentMultiple);
    bool save();
    bool load();
    void reset();
  private:
  	double *_SetPoint;
	double *_Kp;
	double *_Ki;
	double *_Kd;
	double *_Kp2;
	double *_Ki2;
	double *_Kd2;
	bool *_PidDirection;
	int *_SampleTime;
	int *_pid2Band;
	int *_SettingAdjustmentMultiple;
};

#endif
