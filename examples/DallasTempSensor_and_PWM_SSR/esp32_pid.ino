
#define TEMP_SENSOR_PIN 2
#define OUTPUT_PIN 17
#define OUTPUT_PWM_FREQ 490
#define OUTPUT_PWM_RESOLUTION 10 //Resolution 8, 10, 12, 15

#include <OneWire.h>
#include <DallasTemperature.h>
#include "esp32_pid.h"

ESP32PID *myESP32PID;
TaskHandle_t Core2Task;

double Output = 0;
double Input = 0;

OneWire oneWire(TEMP_SENSOR_PIN);  
DallasTemperature sensors(&oneWire);


void mySetup(){
  // Initialize your input and output sensors

  // Initialize Temp sensor
  sensors.begin();

  // Initialize Output Pin for PWM
  pinMode(OUTPUT_PIN, OUTPUT);
  ledcSetup(0, OUTPUT_PWM_FREQ, OUTPUT_PWM_RESOLUTION);
  ledcAttachPin(OUTPUT_PIN, 0);

  myESP32PID->setFailsafe(0,0);
}
void myloop(){
  // Place Code Here That Writes to the Output
  // use the global Output variable which ranges from 0-100
  //
  ledcWrite(0, (Output / 100.0 * 1024) );
  
}
void readInput(){
  // Place code here that reads your input and sets the global Input variable
  // This gets called continuously
  //
  sensors.requestTemperatures();
  Input = sensors.getTempFByIndex(0);
}


/////////////////////////////////////////////////////////////////////////


void setup(void){
  // Initialize ESP32PID and start Background task to continually update input on other core
  myESP32PID = new ESP32PID(&Input, &Output);
  mySetup();
  xTaskCreatePinnedToCore(core2Loop, "Core2Task", 10000, NULL, 0, &Core2Task, 0);
}

void loop(void){
  myESP32PID->loop();
  myloop();
}

void core2Loop( void * pvParameters ){
  for(;;) {
    readInput();
    vTaskDelay( pdMS_TO_TICKS( 10 ) );
  }
  vTaskDelete( NULL );
}



