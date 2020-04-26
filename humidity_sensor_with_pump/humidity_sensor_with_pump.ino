#include <ArduinoJson.h>

// Include Arduino Wire library for I2C
#include <Wire.h>

#define CHECKING_INTERVAL 10000
#define PUMP_RUNNING_TIME 5000 // in ms
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

void checkPumpState() {
  if (millis() % CHECKING_INTERVAL <= 100) {
    Serial.println("Checking");
    if (shouldEnablePump(humidity_sensorA, humidity_sensorB, humidity_sensorC)) {
      Serial.println("enable pump");
      digitalWrite(pumpPin, HIGH);
      delay(PUMP_RUNNING_TIME);
      Serial.println("disable pump");
      digitalWrite(pumpPin, LOW);
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
  // Only process message if there's one
  if (messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(1024); // ArduinoJson version 6+
    // Attempt to deserialize the message
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }


    if (doc["type"] == "request") {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["sensorA"] = humidity_sensorA;
      doc["sensorB"] = humidity_sensorB;
      doc["sensorC"] = humidity_sensorC;
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
