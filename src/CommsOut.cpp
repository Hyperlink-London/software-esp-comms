#include "CommsOut.h"

void CommsOut::send(const String& sensorName, float value) {
    // Stub: no real logic yet
    Serial.printf("[STUB] %s: %.2f\n", sensorName.c_str(), value);
}
