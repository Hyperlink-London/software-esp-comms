#ifndef COMMS_OUT_H
#define COMMS_OUT_H

#include <Arduino.h>
#include <CommsOutInterface.h>  // From interfaces submodule

class CommsOut : public CommsOutInterface {
public:
    void send(const String& sensorName, float value) override;
};

#endif
