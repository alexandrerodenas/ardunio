
void runMeasuresProcess() {
  digitalWrite(pinGreenLed, HIGH);
  sendMeasureRequest();
  DynamicJsonDocument doc = readArduinoJsonResponse();
  const int humidity_sensorA = doc["sensorA"];
  const int humidity_sensorB = doc["sensorB"];
  const int humidity_sensorC = doc["sensorC"];
  sendMeasuresToWebClient(humidity_sensorA, humidity_sensorB, humidity_sensorC);
  digitalWrite(pinGreenLed, LOW);
}

void sendStartPumpCommand() {
  Serial.println("received start pump command");
  digitalWrite(pinGreenLed, HIGH);
  digitalWrite(pinPump, HIGH);
}

void sendStopPumpCommand() {
  Serial.println("received stop pump command");
  digitalWrite(pinGreenLed, LOW);
  digitalWrite(pinPump, LOW);
}
