
#include "esp32_pid.h"
#include "DHT.h"

#define DHTTYPE DHT22 
#define DHTPIN 21
#define RELAY_PIN 17


DHT dht(DHTPIN, DHTTYPE);
ESP32PID *myESP32PID;


double readInput(){
  double h = dht.readHumidity();
  if (isnan(h)) { return 0; }
  return h;
}

void setOutput(double PidOutput){
  digitalWrite(RELAY_PIN, PidOutput/100.0);
}


void setup(void){
  // Initialize ESP32PID
  dht.begin();
  pinMode(RELAY_PIN,OUTPUT);

  myESP32PID = new ESP32PID(readInput, setOutput);
  myESP32PID.useOutputWindow(60);
}

void loop(void){

  myESP32PID->loop();
  
}
