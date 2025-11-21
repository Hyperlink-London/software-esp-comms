#include "CommsOut.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <SerialAliases.h>

typedef struct {
    char sensor[32];
    float value;
} OutboundMsgInternal;

static QueueHandle_t s_outboundQueue = NULL;

CommsOut::CommsOut()
    : espId("UNKNOWN") {
    if (s_outboundQueue == NULL) {
        s_outboundQueue = xQueueCreate(32, sizeof(OutboundMsgInternal));
    }
}

CommsOut::CommsOut(const String& id)
    : espId(id) {
    if (s_outboundQueue == NULL) {
        s_outboundQueue = xQueueCreate(32, sizeof(OutboundMsgInternal));
    }
}

void CommsOut::send(const String& sensorName, float value) {
    if (xPortGetCoreID() == 1 || s_outboundQueue == NULL) {
        this->print(sensorName, value);
        return;
    }
    OutboundMsgInternal msg;
    strncpy(msg.sensor, sensorName.c_str(), sizeof(msg.sensor) - 1);
    msg.sensor[sizeof(msg.sensor) - 1] = '\0';
    msg.value = value;
    xQueueSend(s_outboundQueue, &msg, 0);
}


void CommsOut::drainQueue() {
    if (s_outboundQueue == NULL) return;
    OutboundMsgInternal msg;
    while (xQueueReceive(s_outboundQueue, &msg, 0) == pdTRUE) {
        this->print(msg.sensor, msg.value);
    }
}

void CommsOut::print(const String& sensorName, float value) {
    Serial_1.printf("{\"espId\": \"%s\", \"sensor\": \"%s\", \"value\": %.2f}\n",
        espId.c_str(), sensorName.c_str(), value);
}

void CommsOut::sendAck(const String& command, bool success) {
    Serial_1.printf("{\"espId\": \"%s\", \"ack\": %s, \"command\": \"%s\"}\n",
        espId.c_str(), success ? "true" : "false", command.c_str());
}