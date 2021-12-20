#ifndef myDataLogger
#define myDataLogger
#include "Arduino.h"

#define DATA_ARRAY_SIZE 200
#define EMPTY_DATA 10000
#define ERROR_VALUE 0


class DataLogger
{
  public:
    DataLogger(int log_delay_ms);
    bool logData(double log_data);
    double getItem(int index);
    double min_();
    double max_();
    double getArrayMin(double a[], int length);
    double getArrayMax(double a[], int length);
    double * getDataArray();
  private:
    unsigned long _last_logged;
    int _log_delay_ms;
    double _data_array[DATA_ARRAY_SIZE];
    int _current_pos;
};


#endif
