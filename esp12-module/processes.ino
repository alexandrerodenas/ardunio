void runMeasuresProcess() {
  sendMeasureRequest();
  DynamicJsonDocument doc = readArduinoResponse();
  const int humidity_sensorA = doc["sensorA"];
  const int humidity_sensorB = doc["sensorB"];
  const int humidity_sensorC = doc["sensorC"];
  sendMeasuresToWebClient(humidity_sensorA, humidity_sensorB, humidity_sensorC);
}


void runPumpCommandProcess() {
  sendPumpCommand();
  sendACK();
  pumpState = 1;
  delay(300);
}
