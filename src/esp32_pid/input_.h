#ifndef Input__
#define Input__

#include "Arduino.h"
#include "datatypes.h"
#include "logger.h"

#define SENSOR1_ERROR 1 
#define SENSOR2_ERROR 2 
#define REDUNDANT_SENSOR_MISMATCH 3

class Input_{
  public:
    Input_(double (*readInputFunction_)()); 
    input_state read();
    void useRedundantInput(double (*readInputFunction_)(), int maxDifference, bool useAverage=false);
    unsigned int maxSensorQueryTime = 10000;
  private:
    double (*readInputA)();
    double (*readInputB)();
    double queryInput(double (*readInputFunction_)(), int tries, int sensorId);
    bool redundantInputEnabled = false;
    bool redundantUseAverage = false;
    int redundantMaxDifference;
    double lastReadingA;
    double lastReadingB;
    Logger *logger;

};

#endif
