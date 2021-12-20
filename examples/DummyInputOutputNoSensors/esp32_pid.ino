
#include "esp32_pid.h"
ESP32PID *myESP32PID;

double lastInput = 80;

double readInput(){
  // Place code here that returns the value of your input device
  // This gets called continuously in the background
  lastInput += (random(1,2000)/10000.0 - random(1,2000)/10000.0);
  Serial.println("Input Read: " + String(lastInput));
  delay(5000);
  return lastInput;
}

void setOutput(double PidOutput){
  // Place Code in this function to set the output value
  // the PidOutput variable from 0-100
  Serial.println("Output Set: " + String(PidOutput));
}


void setup(void){
  Serial.begin(9600);
  // Initialize ESP32PID
  struct esp32_pid_settings settings;
  myESP32PID = new ESP32PID(readInput, setOutput);
  myESP32PID->useOutputWindow(10);
}

void loop(void){

  myESP32PID->loop();
  // do not block by using a delay
}
