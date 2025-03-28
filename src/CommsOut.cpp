#include "CommsOut.h"

CommsOut::CommsOut()
    : espId("UNKNOWN") {}

CommsOut::CommsOut(const String& id)
    : espId(id) {}

    void CommsOut::send(const String& sensorName, float value) {
        Serial.printf("{\"espId\": \"%s\", \"sensor\": \"%s\", \"value\": %.2f}\n",
                      espId.c_str(), sensorName.c_str(), value);
    }