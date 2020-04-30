void runMeasuresProcess() {
  sendMeasureRequest();
  DynamicJsonDocument doc = readArduinoJsonResponse();
  const int humidity_sensorA = doc["sensorA"];
  const int humidity_sensorB = doc["sensorB"];
  const int humidity_sensorC = doc["sensorC"];
  sendMeasuresToWebClient(humidity_sensorA, humidity_sensorB, humidity_sensorC);
}

void sendStarPumpCommand() {
  Serial.print("START_PUMP");
  sendResponseToClient("done");
}

void sendStopPumpCommand() {
  Serial.print("STOP_PUMP");
  sendResponseToClient("done");
}
