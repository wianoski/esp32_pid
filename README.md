# About #

The goal of this project is to create super simple yet fully-feature [PID controller](https://en.wikipedia.org/wiki/PID_controller) running on the [TTGO ESP32 microcontroller](https://github.com/Xinyuan-LilyGO/TTGO-T-Display).  The implementation is designed to be extremely flexible, allowing a user to quickly and easily get up and running their choice of imnut and output sennsors/devices.

![](https://bitbucket.org/jason955/esp32_pid/raw/master/img/TTGO.jpg)

### Features ###
* **Interactive GUI** Settings are fully configurable through GUI
* **Webserver** - Remote access over Wifi
* **Charts** - See historical input and output values
* **Failsafe** - Set default state on bad input
* **Redundant input** - Use a second sensor to ensure valid input readings
* **Persistent Settings** - Save settings to EEPROM
* **Dual PIDs** - Agressive and conservative with configurable operating bands
* **Manual Override** - Output can be set manually in the GUI
* **Windowed Output Mode** - For mechanical relays
* **Advanced PID Info** - View interal PID values - for tuning


# Example Projects #

* [**Incubator Temp Controller**](https://bitbucket.org/jason955/esp32_pid/src/master/examples/DallasTempSensor_and_PWM_SSR/) ![](https://bitbucket.org/jason955/esp32_pid/raw/master/examples/DallasTempSensor_and_PWM_SSR/Strip1_600w.jpg)

* [**Humidistat**](https://bitbucket.org/jason955/esp32_pid/src/master/examples/HumidityController/)

* [**No Sensors**](https://bitbucket.org/jason955/esp32_pid/src/master/examples/DummyInputOutputNoSensors/) - Try the project without configuring input and outputs


# Getting Started #

### 1. Prerequisites ###
You will need to have the [Arduino IDE](https://www.arduino.cc/en/software) installed and [set up to work with the ESP32 controller](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/).  You should also have some basic experience working with Arduino and peripherals.


### 2. Configure ###
Download the project to your local machine and use the template located in [esp32_pid/src/esp32_pid/esp32_pid.ino](https://bitbucket.org/jason955/esp32_pid/src/master/src/esp32_pid/esp32_pid.ino):


```c++
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

```

All you need to do is add code the readInput and setOutput functions to read from your input sensor (temp, humidity, level, etc) and write to your output (heater, humidifier, pump, etc). See [Additional Configuration Options](#markdown-header-additional-configuration-options) below.


### 3. Upload ###

Upload the code to your device and that's it!



# Interface #


![](https://bitbucket.org/jason955/esp32_pid/raw/master/img/Buttons.jpg)


**Button 1**

* Short Press - Move Up / Increase Value
* Long Press - Toggle Screen (Home, Large Chart, Advanced Settings)


**Button 2**

* Short Press - Move Down / Decrease Value
* Long Press - Toggle Between Select Mode and Edit Mode



## Modes ##

**Select Mode**

Default mode. When in select mode, you will see one of the menu items on the screen selected in an Orange color.  For example, in the image above the Mode is selected.

You can Short Press Button 1 and Button 2 to move up and down through the selectable items.  The selectible items on this screen are Mode, Setpoint and Gains (P,I,D).

**Edit Mode**

Long Press Button 2 to change to Edit Mode for the selected parameter.  The selected item will turn a magenta color indicating you are in edit mode.  You can short press either of the buttons to increase or decrease the value of the parameter.

To exit Edit Mode and return to Select Mode, long press on either button.



## Screens ##

You can Long Press Button 1 to toggle between screens.  There are 3 screens to toggle between Home, Large Chart and Advanced Settings.

### Home Screen ###

![](https://bitbucket.org/jason955/esp32_pid/raw/master/img/ScreenHome.jpg)

1. **Operating Mode** - Can be toggled between Normal and Manual.
2. **PID Status** - Top line shows which PID is currently in use (Normal or Agressive) and the bottom line shows the values that each of P, I and D is contributing to the output.
3. **Output** - Value the PID is outputing (sum of the items in 2).  This value ranges from 0% - 100%
4. **Current Input Value**
5. **Setpoint** - The value the PID is trying to achieve.
6. **PID Gains** - These are the values the PID is using for the Normal PID.



### Large Chart ###

![](https://bitbucket.org/jason955/esp32_pid/raw/master/img/ScreenLargeChart.jpg)

1. **PID Output** - Value the PID is outputing (sum of the items in 2).  This value ranges from 0% - 100%
2. **Internal PID Values** - Indicates the values that each of P, I and D is contributing to the output.  This is used for troubleshooting your PID.
3. **PID In Use** - Indicates which PID is currently in use (Normal or Agressive).


### Advanced Settings ###

![](https://bitbucket.org/jason955/esp32_pid/raw/master/img/ScreenAdvSettings.jpg)

1. **PID Cycle Time** - This is the time in milliseconds the PID is waiting before performing a new calculation.  In this example, the pid is computing every 10 seconds.
2. **PID Direction** - Direct or Reverse.  If an increase in output increases the input value, select Direct.  If an increase in output decreases the input value, select Reverse.
3. **Agressive PID Gains** - These are the values of P, I and D for the Agressive PID.
4. **Agressive PID Operating Band** - This setting determines when the Agressive PID will kick in.  In the image the band is 5, meaning if the input is 5 units or more away from the output, the system will use the Agressive PID. To further clarify, if the band is set to 5, the output is set to 80 and the input is 86, the system will use the Agressive pid.  If the input is 84, the system will use the Normal PID (setting on the home screen). 
5. **Setting Adjustment Multiplier** - This is the amount the parameters will change in Edit Mode.  For example the default adjustment to the Output is 1 up or down.  You can change this setting to 10 and the parameter will change by 10 each adjustment up or down.
6. ....
7. **Reset PIDs** - Sometimes this needs to be cleared out if you have I windup.
8. **Save Settings** - Save all settings to EEPROM



# Additional Configuration Options #


### Web Server ###

You can implement basic failsafe functionality by specifing the minimum and maximum valid sensor values and a default output value.  If the sensor value falls outside this range, the output will default the failsafe output state.  This can prevent an overheating in a heating PID in the event a sensor is disconnected.

```c++
  // Load Wi-Fi library
  #include <WiFi.h>
  // Replace with your network credentials
  const char* ssid = "mywifi";
  const char* password = "mypasswd";


  // in the setup
  myESP32PID = new ESP32PID(readInput, setOutput);
  myESP32PID->enableWebServer();
  WiFi.begin(ssid, password);


```

### Failsafe ###

You can implement basic failsafe functionality by returning the value of NAN from the input function.  You can set the minimum and maximum valid sensor values and if the sensor value falls outside this range, the output will default the failsafe output state.  This can prevent an overheating in a heating PID in the event a sensor is disconnected.

```c++
  double readInput(){
  double value = sensor.read();
  if(value<0 || value>1000){
    return nan(""); // returning NAN indicates an error 
  }
  return value;
}

  myESP32PID = new ESP32PID(readInput, setOutput);
  // you can specify the output 0-100 (default 0) in the event of a sensor disagreement
  myESP32PID->errorOutputValue = 50;


```

### Redundant Sensor ###

You can use a redundant input sensor to ensure the sensor is reading correctly.  You can spedifiy if the primary sensor value or the average of the two sensor values sould be used as input to the controller.

```c++
  myESP32PID = new ESP32PID(readInput, setOutput);
  double readInputRedundant(){
    // Redundant sensor should return its value
    return mySensor.getTemp();
  }
  int maxSensorDifference = 5; // If the sensors are this far apart there's an error


  // Defaults to using primary sensor as input and 0 as the default output on error
  myESP32PID->useRedundantInput(readInputRedundant, maxSensorDifference);


  // You can provide a third parameter to use the average of the two sensors, vs the primary
  bool useAverage = true;
  myESP32PID->useRedundantInput(readInputRedundant, maxSensorDifference, useAverage);


  // you can specify the output 0-100 (default 0) in the event of a sensor disagreement
  myESP32PID->errorOutputValue = 50;

  
```

### Windowed Mode ###

If your output is a mechanical relay, it can only output on or off.  You can use Windowed mode to turn on and off the relay for a percentage of a window period you define.

For example with a window size of 100 seconds and a PID output value of 25%, output will be switched (output set to 100) on for 25 seconds and then off (output set to 0) for 75 seconds.

```c++
  myESP32PID = new ESP32PID(readInput, setOutput);
  myESP32PID.useOutputWindow(100); // use a window period of 100 seconds.
```


### Advanced Parameters ###

You can pass a third parameter when creating a new ESP32PID object that takes a struct contining your settings.  

```c++

struct esp32_pid_settings settings;

settings.SetPoint = 100;
myESP32PID = new ESP32PID(readInput, setOutput, settings);


// Available Settings: 
// IMPORTANT - If these values have already been written to the EEPROM, the EEPROM values will be used.
//
// double SetPoint = 80;
// bool PidDirection = DIRECT;
// bool OperatingMode = AUTOMATIC;
// int SampleTime = 1000;
// double Kp=5;
// double Ki=0;
// double Kd=0;
// double Kp2=10;
// double Ki2=0;
// double Kd2=0;
// int pid2Band = 10;
// int dataLogDelay = 10000; // 10 seconds - gives about 40 minutes of chart
// int SettingAdjustmentMultiple = 1;
// unsigned int maxSensorQueryTime = 10000; // max amount of time in MS to spend trying to get valid result from each sensor


```

