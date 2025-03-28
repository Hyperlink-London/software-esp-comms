#ifndef COMMS_OUT_H
#define COMMS_OUT_H

#include <Arduino.h>

class CommsOut {
public:
    void send(const String& sensorName, float value);
};

#endif
