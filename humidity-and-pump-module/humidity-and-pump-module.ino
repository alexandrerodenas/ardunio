#include <ArduinoJson.h>

// Include Arduino Wire library for I2C
#include <Wire.h>

#define CHECKING_INTERVAL 10000
#define PUMP_RUNNING_TIME 15000 // in ms
#define SENSOR_THRESHOLD 65

// Define Slave I2C Address
#define SLAVE_ADDR 9

// Define Slave answer size
#define ANSWERSIZE 3

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
  // Initialize I2C communications as Master
  Wire.begin();
  Serial.println("Init");
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
    DynamicJsonDocument doc(1024); // ArduinoJson version 6+
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }

    Serial.println(message);

    if (doc["type"] == "request") {
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

    messageReady = false;
  }
}

void loop() {
  refreshSensorValues();
  handleMonitorSerialCommunication();
  //printSensorValues();
  //checkPumpState();
  //Wire.beginTransmission(SLAVE_ADDR);
  //Wire.write(humidity_sensorA);
  //Wire.write(humidity_sensorB);
  //Wire.write(humidity_sensorC);
  //Wire.endTransmission();

  //Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);


}
