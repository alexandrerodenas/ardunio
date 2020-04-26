void sendMeasuresToWebClient(int measureSensorA, int measureSensorB, int measureSensorC) {
  const size_t CAPACITY = JSON_ARRAY_SIZE(3);

  StaticJsonDocument<CAPACITY> doc;

  JsonArray array = doc.to<JsonArray>();

  // add some values
  array.add(measureSensorA);
  array.add(measureSensorB);
  array.add(measureSensorC);

  sendJsonToWebClient(doc);
}

void sendPumpState() {
  DynamicJsonDocument doc(1024);
  doc["state"] = pumpState;
  sendJsonToWebClient(doc);
}


void sendJsonToWebClient(DynamicJsonDocument doc) {
  char output[128];
  serializeJson(doc, output);
  server.send(200, "application/json", output);
}
