DynamicJsonDocument readArduinoJsonResponse() {
  DynamicJsonDocument doc(1024);

  // Reading the response
  boolean messageReady = false;
  String message = "";
  while (messageReady == false) {
    if (Serial.available()) {
      message = Serial.readString();
      messageReady = true;
    }
  }
  // Attempt to deserialize the JSON-formatted message
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print(F("ESP12 - SYNC deserializeJson() failed: "));
    Serial.println(error.c_str());
  }

  return doc;
}

String readArduinoSerialResponse(int timeout) {
  boolean messageReady = false;
  String message = "";
  int waitingTime = 0;
  while (messageReady == false && waitingTime < timeout) {
    if (Serial.available()) {
      message = Serial.readString();
      return message;
    }
  }

  return "NOK";
}



void sendMeasureRequest() {
  DynamicJsonDocument doc(1024);
  doc["type"] = "request";
  serializeJson(doc, Serial);
}
