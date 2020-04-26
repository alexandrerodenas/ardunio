DynamicJsonDocument readArduinoResponse() {
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
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  }

  return doc;
}

DynamicJsonDocument readAsyncArduinoResponse() {
  DynamicJsonDocument doc(1024);

  if (Serial.available()) {
    String message = Serial.readString();
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
    }
  }

  return doc;
}

void sendPumpCommand() {
  DynamicJsonDocument doc(1024);
  doc["type"] = "command";
  doc["subtype"] = "pump";
  serializeJson(doc, Serial);
}

void sendMeasureRequest() {
  DynamicJsonDocument doc(1024);
  doc["type"] = "request";
  serializeJson(doc, Serial);
}
