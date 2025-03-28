#include "CommsOut.h"

void CommsOut::send(const String& sensorName, float value) {
    // Example implementation using Serial
    Serial.printf("CommsOut - Sensor [%s]: %.2f\n", sensorName.c_str(), value);
}
