
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <math.h>

#define ssid      "SFR-e600"
#define password  "RodeTrip@12"

const short int pinPump = 16; // GPIO16 D3
const short int pinRedLed = 5; // GPIO5 D1
const short int pinGreenLed = 0; // GPIO0 D0


ESP8266WebServer server ( 80 );

void setup() {
  Serial.begin ( 9600 );
  Serial.println("init esp12");
  WiFi.begin ( ssid, password );
  // Wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  //WiFi connexion is OK
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  pinMode(pinPump, OUTPUT);
  pinMode(pinRedLed, OUTPUT);
  pinMode(pinGreenLed, OUTPUT);

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


  server.on("/pump/stop", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    sendStopPumpCommand();
  });


  server.on("/pump/start", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    sendStartPumpCommand();
  });

  // Démarre le serveur web - Start Web Server
  server.begin();
  Serial.println ( "HTTP server started" );
}


void loop() {
  // Regularly checks the connection of new clients
  server.handleClient();
}
