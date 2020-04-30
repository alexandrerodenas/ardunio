#include <ArduinoJson.h>

#define CHECKING_INTERVAL 10000
#define PUMP_RUNNING_TIME 15000 // in ms
#define SENSOR_THRESHOLD 65

const int pumpPin = 2;

int pumpWaitingTime = 0;

int humidity(int rawValue) {
  return (1 - rawValue / 1024.0) * 100.0;
}

int humidity_sensorA = humidity(analogRead(A0));
int humidity_sensorB = humidity(analogRead(1));
int humidity_sensorC = humidity(analogRead(2));

void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  Serial.println("Init Arduino");
}


boolean shouldEnablePump(int valueSensorA, int valueSensorB, int valueSensorC) {
  return (valueSensorA <= SENSOR_THRESHOLD || valueSensorB <= SENSOR_THRESHOLD || valueSensorC <= SENSOR_THRESHOLD);
}

void refreshSensorValues() {
  humidity_sensorA = humidity(analogRead(A0));
  humidity_sensorB = humidity(analogRead(1));
  humidity_sensorC = humidity(analogRead(2));
}

void printSensorValues() {
  Serial.println(humidity_sensorA);
  Serial.println(humidity_sensorB);
  Serial.println(humidity_sensorC);
}

void runPump(int time) {
  digitalWrite(pumpPin, HIGH);
  delay(time);
  digitalWrite(pumpPin, LOW);
}

void checkPumpState() {
  if (millis() % CHECKING_INTERVAL <= 100) {
    Serial.println("Checking");
    if (shouldEnablePump(humidity_sensorA, humidity_sensorB, humidity_sensorC)) {
      runPump(PUMP_RUNNING_TIME);
    }
  }
}

void handleMonitorSerialCommunication() {
  bool messageReady = false;
  String message = "";

  while (Serial.available()) {
    message = Serial.readString();
    messageReady = true;
  }

  if (messageReady) {
    if (message == "START_PUMP") {
      digitalWrite(pumpPin, HIGH);
    } else if (message == "STOP_PUMP") {
      digitalWrite(pumpPin, LOW);
    } else {
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, message);
      if (error) {
        Serial.print(F("Arduino deserializeJson() failed: "));
        Serial.println(error.c_str());
        messageReady = false;
        return;
      }

      if (doc["type"] == "request") {
        refreshSensorValues();
        doc["type"] = "response";
        // Get data from analog sensors
        doc["sensorA"] = humidity_sensorA;
        doc["sensorB"] = humidity_sensorB;
        doc["sensorC"] = humidity_sensorC;
        serializeJson(doc, Serial);
      }

      if (doc["type"] == "command" && doc["subtype"] == "pump") {
        doc["type"] = "response";
        doc["status"] = "IN_PROGRESS";
        serializeJson(doc, Serial);
        runPump(PUMP_RUNNING_TIME);
        doc["status"] = "DONE";
        serializeJson(doc, Serial);
      }
    }
    messageReady = false;
  }
}

void loop() {
  handleMonitorSerialCommunication();
}
