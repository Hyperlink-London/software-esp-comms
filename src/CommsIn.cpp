#include "CommsIn.h"
#include <Arduino.h>
#include <ArduinoJson.h>

void CommsIn::receive() {
    if (Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');
        message.trim(); // Remove any whitespace
        
        if (message.length() > 0) {
            // Create a JSON document
            StaticJsonDocument<200> doc;
            
            // Try to parse the JSON
            DeserializationError error = deserializeJson(doc, message);
            
            if (error) {
                Serial.print("Error: Incomplete or invalid JSON message - ");
                Serial.println(error.c_str());
            } else {
                Serial.print("Received valid JSON message: ");
                Serial.println(message);
            }
        }
    }
}
