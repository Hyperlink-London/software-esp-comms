#include "CommsIn.h"
#include <Arduino.h>
#include <SerialAliases.h>
#include <ArduinoJson.h>

void CommsIn::receive() {
    if (Serial_1.available() > 0) {
        String message = Serial_1.readStringUntil('\n');
        message.trim(); // Remove any whitespace
        
        if (message.length() > 0) {
            // Create a JSON document
            StaticJsonDocument<200> doc;
            
            // Try to parse the JSON
            DeserializationError error = deserializeJson(doc, message);
            
            if (error) {
                Serial_1.print("Error: Incomplete or invalid JSON message - ");
                Serial_1.println(error.c_str());
            } else {
                Serial_1.print("Received valid JSON message: ");
                Serial_1.println(message);
            }
        }
    }
}
