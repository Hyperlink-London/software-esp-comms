#ifndef COMMS_IN_H
#define COMMS_IN_H

#include <Arduino.h>
#include <CommsInInterface.h>

class CommsIn : public CommsInInterface {
private:
    unsigned long lastReciveTime;
    static const unsigned long MAX_SILENT_PERIOD_MS = 5000;

public:
    void receive() override;
    bool isStale();
};

#endif
