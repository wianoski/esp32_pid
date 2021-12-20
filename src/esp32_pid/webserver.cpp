#include "Arduino.h"
#include "webserver.h"
#include "data_logger.h"
#include "webpages.h"

#define WIFI_CONNECTION_CHECK_TIMEOUT 10000

// Set web server port number to 80
WiFiServer myserver(80);


#define ROUTES_LEN 5
Route myRoutes[ROUTES_LEN] = {  
                                {"GET", "/variables", *variables_get},
                                {"GET", "/chart_data", *chart_data},
                                {"GET", "/last_chart_data", *last_chart_data},
                                {"GET", "/asdf", *route_test},
                                {"GET", "/ ", *route_home }
                             };




WebServer::WebServer( input_state *inputState_,
                      struct esp32_pid_settings *settings,
                      struct pid_state_values *pidState,
                      bool *resetPID,bool *saveSettings,
                      DataLogger *inputLog, DataLogger *setpointLog, DataLogger *outputLog)
{

  inputState = inputState_;
  _settings = settings;
  _pidState = pidState;
  _resetPID = resetPID;
  _saveSettings = saveSettings;
  _inputLog = inputLog;
  _setpointLog = setpointLog;
  _outputLog = outputLog;
  logger = new Logger("Webserver");
}

void WebServer::sendResponseHeaders(){
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Connection: close");
  client.println();
};

void WebServer::send404(){
  logger->log(LOG_LEVEL_DEBUG, "send404", "No route found for " + header + " , sending 404");
  client.println("HTTP/1.1 404 Not Found");
  client.println();
}

void WebServer::parseRoute(){
  for(int i=0; i<ROUTES_LEN; i++){
    if(header.indexOf(myRoutes[i].requestType + " " + myRoutes[i].route) == 0 ){
      logger->log(LOG_LEVEL_DEBUG, "send404", "Found Matching Route: " + myRoutes[i].requestType + " " + myRoutes[i].route);
      sendResponseHeaders();
      (*myRoutes[i].handler)(*this);
      return;
    }
  }
  send404();

};

void WebServer::checkServerOnline(){
  if(millis() - lastWifiConnectionCheck < WIFI_CONNECTION_CHECK_TIMEOUT){
    // Only check every so often based on WIFI_CONNECTION_CHECK_TIMEOUT var
    return;
  }
  lastWifiConnectionCheck = millis();

  if(WiFi.status() != WL_CONNECTED){
    if(serverStarted){
      // WIFI previously connected but now error, RESTART
      logger->log(LOG_LEVEL_WARN, "checkServerOnline", "WIFI Connection lost!  Reconnecting...");
      WiFi.disconnect();
      WiFi.reconnect();
      serverStarted = false;
    }else{
      // Server not started yet, waiting for initial wifi connection
      logger->log(LOG_LEVEL_WARN, "checkServerOnline", "Error Starting Server, No WIFI Connection!");
    }

  }
  if(!serverStarted && WiFi.status() == WL_CONNECTED){
    // server not started but now we have wifi, so start server.
    myserver.begin();
    logger->log(LOG_LEVEL_INFO, "checkServerOnline", "Server Started. IP address: " + WiFi.localIP().toString() );
    serverStarted = true;
  }



    


}


#define MAX_BLOCK_TIME 10

void WebServer::readRequest(){

  // check for listening clients
  // read input for x milliseconds
  // if response needed, call read response
  // if no more response needed, kill connection

  if(!currentlyReceiving){
    client = myserver.available();
    if(client){ 
        currentlyReceiving = true; 
        clientConnectStartTime = millis();
        lastReceiveTime = millis();
        logger->log(LOG_LEVEL_DEBUG, "readRequest", "New Connection");
      }
    else{ return; }
  }

  currentResponseStartTime = millis();
  
  while(client.connected() && millis() - currentResponseStartTime <= MAX_BLOCK_TIME) {
    if (client.available()) {
      char c = client.read();             // read a byte, then
      header += c;
      lastReceiveTime = millis();
      // Serial.print(c);
      if (c == '\n') {
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          parseRoute();
          client.println();
          closeConnection();
          return;

        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }

  if(millis()-lastReceiveTime > timeoutTime){
    closeConnection();
  }

 

}

void WebServer::sendResponse(){

}
void WebServer::closeConnection(){
  header = "";
  currentLine = "";
  currentlyReceiving = false;
  client.stop();
  logger->log(LOG_LEVEL_DEBUG, "closeConnection", "Connection closed total time: " + String(millis()-clientConnectStartTime));
}


void WebServer::processRequests(){
  checkServerOnline();
  readRequest();
}
