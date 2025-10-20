#ifndef COMMS_OUT_H
#define COMMS_OUT_H

#include <Arduino.h>
#include <CommsOutInterface.h>

class CommsOut : public CommsOutInterface {
private:
    String espId;

public:
    CommsOut();
    CommsOut(const String& id);

    void send(const String& sensorName, float value) override;
    void drainQueue();
};

#endif
