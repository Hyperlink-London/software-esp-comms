#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

//size buffer queue for out of order messages
#define QUEUE_SIZE 10

class SerialComm 
{
public:
    SerialComm();
    void setupTasks();
    static void taskProcessMessage(void *param);//core 1 process incoming messages
    static void taskHandleMessage(void *param);//core 2 handle logic

private:
    static int calculateChecksum(const String &message); //XOR od ASCII values
    static void sendResponse(int id, int data);
    static void requestMissingIDs(int lastReceivedID);
    static QueueHandle_t messageQueue;// message sharing between tasks
    static int lastReceivedID;//last validID
};

#endif
