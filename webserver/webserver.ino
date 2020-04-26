
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <math.h>

#define ssid      "SFR-e600"
#define password  "RodeTrip@12"


ESP8266WebServer server ( 80 );


void sendMeasuresOnWeb(int measureSensorA, int measureSensorB, int measureSensorC) {
  // compute the required size
  const size_t CAPACITY = JSON_ARRAY_SIZE(3);

  // allocate the memory for the document
  StaticJsonDocument<CAPACITY> doc;

  // create an empty array
  JsonArray array = doc.to<JsonArray>();

  // add some values
  array.add(measureSensorA);
  array.add(measureSensorB);
  array.add(measureSensorC);

  char output[128];

  // serialize the array and send the result to Serial
  serializeJson(doc, output);

  server.send(200, "application/json", output);
  Serial.println("Tableau mesures envoyees");
}

void sendMeasureRequestToArduino() {
  // Send a JSON-formatted request with key "type" and value "request"
  // then parse the JSON-formatted response with keys "gas" and "distance"
  DynamicJsonDocument doc(1024);
  // Sending the request
  doc["type"] = "request";
  serializeJson(doc, Serial);
}

void runMeasuresProcess() {
  sendMeasureRequestToArduino();
  DynamicJsonDocument doc(1024);

  // Reading the response
  boolean messageReady = false;
  String message = "";
  while (messageReady == false) { // blocking but that's ok
    if (Serial.available()) {
      message = Serial.readString();
      messageReady = true;
    }
  }
  // Attempt to deserialize the JSON-formatted message
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  const int humidity_sensorA = doc["sensorA"];
  const int humidity_sensorB = doc["sensorB"];
  const int humidity_sensorC = doc["sensorC"];

  sendMeasuresOnWeb(humidity_sensorA, humidity_sensorB, humidity_sensorC);
}

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

  // Serve files
  //server.serveStatic("/images", SPIFFS, "/images");
  //server.serveStatic("/js", SPIFFS, "/js");

  //server.serveStatic("/", SPIFFS, "/index.html");
  //server.serveStatic("/styles.css", SPIFFS, "/styles.css");
  //server.sendHeader("Access-Control-Allow-Origin", "*");

  server.on("/measures.json", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    runMeasuresProcess();
  });


  // Démarre le serveur web - Start Web Server
  server.begin();
  Serial.println ( "HTTP server started" );

}


void loop() {
  // Regularly checks the connection of new clients
  server.handleClient();
  delay(100);
}
