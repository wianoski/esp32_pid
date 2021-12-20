#include "esp32_pid.h"
ESP32PID *myESP32PID;


double readInput(){
  // Place code here that returns the value of your input device
  // This gets called continuously in the background
  // return  mySensor.getTemp();
}
void setOutput(double PidOutput){
  // Place Code in this function to set the output value
  // the PidOutput variable from 0-100
  // myHeater->setOutput( PidOutput / 100.0 );
}


void setup(void){
  // Initialize your input and output sensors
  // mySensor = new Sensor();
  // myHeater = new OutputDevice();
  
  // Initialize ESP32PID
  myESP32PID = new ESP32PID(readInput, setOutput);
}
void loop(void){
  myESP32PID->loop();
}
