#ifndef WebServer_
#define WebServer_

#include "Arduino.h"
#include "datatypes.h"
#include <WiFi.h>
#include "data_logger.h"
#include "logger.h"



class WebServer{
  public:
    WebServer(input_state *inputState_,
              struct esp32_pid_settings *,
              struct pid_state_values *,
              bool *resetPID, bool *saveSettings,
              DataLogger *inputLog, DataLogger *setpointLog, DataLogger *outputLog
      );

    void processRequests();
    struct input_state *inputState;
    struct esp32_pid_settings *_settings;
    struct pid_state_values *_pidState;
    bool *_resetPID;
    bool *_saveSettings;
    DataLogger *_inputLog;
    DataLogger *_setpointLog;
    DataLogger *_outputLog;
    WiFiClient client;

  private:
    void sendResponseHeaders();
    void send404();
    void parseRoute();
    void checkServerOnline();
    void readRequest();
    void sendResponse();
    void closeConnection();
    bool currentlyReceiving = false;
    String currentLine = "";
    unsigned long clientConnectStartTime;
    unsigned long currentResponseStartTime;
    unsigned long lastReceiveTime;
    // unsigned long requestStartTime;
    
    unsigned long currentTime;
    unsigned long previousTime;
    int timeoutTime = 250;
    String header;
    bool serverStarted = false;
    unsigned long lastWifiConnectionCheck = 0;
    bool wifiLastOnline = false;
    Logger *logger;
};

struct Route{
  String requestType;
  String route;
  void (*handler)(WebServer& server);
};





#endif
