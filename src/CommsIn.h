#ifndef COMMS_IN_H
#define COMMS_IN_H

#include <Arduino.h>
#include <CommsInInterface.h>

class CommsIn : public CommsInInterface {
public:
    void receive() override;
};

#endif
