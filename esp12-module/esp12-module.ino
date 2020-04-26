
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <math.h>

#define ssid      "SFR-e600"
#define password  "RodeTrip@12"


ESP8266WebServer server ( 80 );

int pumpState = 0; // 0 = NOT WORKING; 1 = IN PROGRESS

void setup() {
  Serial.begin ( 9600 );
  Serial.println("init");
  WiFi.begin ( ssid, password );
  // Wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  //WiFi connexion is OK
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  if (!SPIFFS.begin())
  {
    // Serious problem
    Serial.println("SPIFFS Mount failed");
  } else {
    Serial.println("SPIFFS Mount succesfull");
  }

  server.on("/measures.json", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    runMeasuresProcess();
  });


  server.on("/pump", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    sendPumpCommand();
  });


  server.on("/pump/state", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    sendPumpState();
  });
  
  // Démarre le serveur web - Start Web Server
  server.begin();
  Serial.println ( "HTTP server started" );
}


void loop() {
  // Regularly checks the connection of new clients
  server.handleClient();
  DynamicJsonDocument doc = readAsyncArduinoResponse();
  if(doc["type"] == "response"){
    if(doc["subtype"] == "pump"){
      if(doc["status"] == "DONE"){
        pumpState = 0;
      } else {
        pumpState = 1;
      }
    }
  }
  delay(100);
}
