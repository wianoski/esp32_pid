#include "Arduino.h"
#include "esp32_pid.h"


Input_ *myInput;
input_state inputState;
TaskHandle_t Core2Task;


void core2Loop( void * pvParameters ){
  for(;;) {
    inputState = myInput->read();
    vTaskDelay( pdMS_TO_TICKS( 10 ) );
  }
  vTaskDelete( NULL );
}


ESP32PID::ESP32PID(double (*readInputF)(), void (*setOutputF)(double), struct esp32_pid_settings settings)
{
  _settings = settings;
  initialize(readInputF, setOutputF);
}

ESP32PID::ESP32PID(double (*readInputFunction_)(), void (*setOutputFunction_)(double) )
{
  initialize(readInputFunction_,setOutputFunction_);
}

void ESP32PID::useOutputWindow(int windowLength){
  outputWindow.use = true;
  outputWindow.windowLengthMs = windowLength * 1000UL;
  outputWindow.cycleStartTime = millis();
}

void ESP32PID::useRedundantInput(double (*readInputFunction_)(), int maxDifference, bool useAverage){
  myInput->useRedundantInput(readInputFunction_,  maxDifference, useAverage);
}

void ESP32PID::initialize(double (*readInputFunction_)(), void (*setOutputFunction_)(double)){
  last.Kp = _settings.Kp;
  last.Ki = _settings.Ki;
  last.Kd = _settings.Kd; 
  last.Kp2 = _settings.Kp2;
  last.Ki2 = _settings.Ki2; 
  last.Kd2 = _settings.Kd2; 
  last.OperatingMode = _settings.OperatingMode;
  last.PidDirection = _settings.PidDirection;
  last.SampleTime = _settings.SampleTime;

  setOutputFunction = setOutputFunction_;

  mySettings = new SettingsStore(&_settings.SetPoint, &_settings.Kp, &_settings.Ki, &_settings.Kd, &_settings.Kp2, &_settings.Ki2, &_settings.Kd2, &_settings.PidDirection, &_settings.SampleTime, &_settings.pid2Band, &_settings.SettingAdjustmentMultiple);
  mySettings->load();
  myInput = new Input_(readInputFunction_);
  myInput->maxSensorQueryTime = _settings.maxSensorQueryTime;
  inputLog = new DataLogger(_settings.dataLogDelay);
  setPointLog = new DataLogger(_settings.dataLogDelay);
  outputLog = new DataLogger(_settings.dataLogDelay);
  oLed = new Display(200, &inputState, &_settings.SetPoint, &myOutput, &_settings.Kp, &_settings.Ki, &_settings.Kd, &_settings.Kp2, &_settings.Ki2, &_settings.Kd2, &_settings.pid2Band, &_settings.OperatingMode, &_settings.SampleTime, &_settings.PidDirection, &pidState.PidOutputP, &pidState.PidOutputI, &pidState.PidOutputD, &pidState.ManualOutput, &resetPid, &saveSettings, &pidState.UsePrimaryPID, &_settings.SettingAdjustmentMultiple, inputLog, setPointLog, outputLog);
  controls = new Controls(0,35,100,500);
  myPID = new PID(&inputState.value, &pidState.PidOutputSum, &_settings.SetPoint, &pidState.PidOutputP, &pidState.PidOutputI, &pidState.PidOutputD, _settings.Kp, _settings.Ki, _settings.Kd, _settings.PidDirection);
  myPID->SetMode(_settings.OperatingMode);
  myPID->SetSampleTime(_settings.SampleTime);
  myPID->SetOutputLimits(0, 100);
  
  xTaskCreatePinnedToCore(core2Loop, "Core2Task", 10000, NULL, 0, &Core2Task, 0);
}

void ESP32PID::loop()
{
  syncSettings();
  float output;

  if(inputState.error && _settings.OperatingMode == AUTOMATIC){
     output = errorOutputValue;
  }
  else if(_settings.OperatingMode == AUTOMATIC){
    myPID->Compute();
    output = pidState.PidOutputSum;
  }
  else if(_settings.OperatingMode == MANUAL){
    output = double(pidState.ManualOutput);
  }

  oLed->processControlInput(controls->getState());
  oLed->update_();

  if(outputWindow.use){
    myOutput = calculateWindowOutput(output);
  }
  else{
    myOutput = output;
  }
  
  if(myOutput != last.myOutput){
    last.myOutput = myOutput;
    setOutputFunction(myOutput);
  }

  inputLog->logData(inputState.value);
  setPointLog->logData(_settings.SetPoint);
  outputLog->logData(output);

  if(webServerEnabled){
    myWebServer->processRequests();
  }
}

double ESP32PID::calculateWindowOutput(double output){
  
    outputWindow.cycleTimeElapsed = millis() - outputWindow.cycleStartTime;


    if( outputWindow.cycleTimeElapsed >= outputWindow.windowLengthMs){ 
      outputWindow.cycleStartTime = millis();
      // Serial.println("resetting cycle time");
      outputWindow.cycleOnTime = output/100.0*outputWindow.windowLengthMs;
      outputWindow.cycleTimeElapsed = millis() - outputWindow.cycleStartTime;
    }

    if (outputWindow.cycleTimeElapsed < outputWindow.cycleOnTime ){
      return 100;
    }else{
      return 0;
    }
  }


void ESP32PID::enableWebServer(){
  webServerEnabled = true;
  myWebServer = new WebServer(&inputState, &_settings, &pidState, &resetPid, &saveSettings, inputLog, setPointLog, outputLog);
}


void ESP32PID::syncSettings(){

  if(abs(inputState.value - _settings.SetPoint) < _settings.pid2Band ){ pidState.UsePrimaryPID = true;} else{pidState.UsePrimaryPID = false;}

  if(resetPid){
    myPID->Reset();
    resetPid = false;
  }
  if(saveSettings){
    mySettings->save();
    saveSettings = false;
  }
  if(last.OperatingMode != _settings.OperatingMode){
     myPID->SetMode(_settings.OperatingMode);
     last.OperatingMode = _settings.OperatingMode;
  }
  if(last.PidDirection != _settings.PidDirection){
    myPID->SetControllerDirection(_settings.PidDirection);
    last.PidDirection = _settings.PidDirection;
  }
  if(last.SampleTime != _settings.SampleTime){
    myPID->SetSampleTime(_settings.SampleTime);
    last.SampleTime = _settings.SampleTime;
  } 

  if( (last.Kp != _settings.Kp || last.Ki != _settings.Ki || last.Kd != _settings.Kd || last.UsePrimaryPID != pidState.UsePrimaryPID) && pidState.UsePrimaryPID){
    myPID->SetTunings(_settings.Kp,_settings.Ki,_settings.Kd);
    last.Kp = _settings.Kp;
    last.Ki = _settings.Ki;
    last.Kd = _settings.Kd;
  }

  if((last.Kp2 != _settings.Kp2 || last.Ki2 != _settings.Ki2 || last.Kd2 != _settings.Kd2 || last.UsePrimaryPID != pidState.UsePrimaryPID) && !pidState.UsePrimaryPID){
    myPID->SetTunings(_settings.Kp2,_settings.Ki2,_settings.Kd2);
    last.Kp2 = _settings.Kp2;
    last.Ki2 = _settings.Ki2;
    last.Kd2 = _settings.Kd2;
  }
  last.UsePrimaryPID = pidState.UsePrimaryPID;
}



pid_state_values ESP32PID::getPidState(){
  pid_state_values tempPidState;
  tempPidState = pidState;
  return tempPidState;
}


input_state ESP32PID::getInputState(){
  input_state temp_input_state;
  temp_input_state = inputState;
  return temp_input_state;
}
esp32_pid_settings ESP32PID::getSettings(){
  esp32_pid_settings temp_esp32_pid_settings;
  temp_esp32_pid_settings = _settings;
  return temp_esp32_pid_settings;
}

double ESP32PID::getOutput(){
  return myOutput;
}

void ESP32PID::setSP(double sp){
  _settings.SetPoint = sp;
}


