#include "SerialComm.h"

QueueHandle_t SerialComm::messageQueue = nullptr;
int SerialComm::lastReceivedID = 0;

SerialComm::SerialComm() {}

void SerialComm::setupTasks() 
{
    messageQueue = xQueueCreate(QUEUE_SIZE, sizeof(String));//queue for message sharing
    if (messageQueue == nullptr) 
    {
        Serial.println("Failed to create message queue!");
        return;
    }
    //freeRTOS tasks for C1 C2
    xTaskCreatePinnedToCore(taskProcessMessage, "TaskProcessMessage", 4096, nullptr, 1, nullptr, 0);
    xTaskCreatePinnedToCore(taskHandleMessage, "TaskHandleMessage", 4096, nullptr, 1, nullptr, 1);
}

//reading and enqueuing oncoming messages
void SerialComm::taskProcessMessage(void *param) 
{
    ///should be modified  
    while (true) 
    {
        if (Serial.available()) 
        {
            String incomingMessage = Serial.readStringUntil('\n'); //read until newline
            incomingMessage.trim(); //remove whitespace at end
            //check format
            if (!incomingMessage.startsWith("<")||!incomingMessage.endsWith(">")) 
            {
                Serial.println("Invalid message format.");
                continue;
            }
            //enqueue valid message
            if (xQueueSend(messageQueue, &incomingMessage, pdMS_TO_TICKS(100)) != pdTRUE) 
            {
                Serial.println("Message queue is full. Dropping message.");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

//process and validate message
void SerialComm::taskHandleMessage(void *param) 
{
    String incomingMessage;
    //should be modified
    while (true) 
    {
        //wait for message
        if (xQueueReceive(messageQueue, &incomingMessage, portMAX_DELAY) == pdTRUE) {
            //parsemessage
            int idStart = incomingMessage.indexOf('<') + 1;
            int idEnd = incomingMessage.indexOf(':');
            int dataEnd = incomingMessage.lastIndexOf(':');
            int checksumEnd = incomingMessage.indexOf('>');
            if (idStart < 0 || idEnd < 0 || dataEnd < 0 || checksumEnd < 0) 
            {
                Serial.println("Invalid message format.");
                continue;
            }
            String idStr = incomingMessage.substring(idStart, idEnd);
            String dataStr = incomingMessage.substring(idEnd + 1, dataEnd);
            String checksumStr = incomingMessage.substring(dataEnd + 1, checksumEnd);
            int id = idStr.toInt();
            int data = dataStr.toInt();
            int receivedChecksum = checksumStr.toInt();
            //recalculate checksum 
            String validationString = idStr+":"+dataStr;
            int calculatedChecksum = calculateChecksum(validationString);
            if (calculatedChecksum != receivedChecksum) 
            {
                Serial.println("checksum mismatch hense ignore.");
                continue;
            }
            //check for missingIDs
            if (id !=lastReceivedID+ 1) 
            {
                requestMissingIDs(lastReceivedID);
            }
            lastReceivedID = id;
            int result = data+10; //modify later
            sendResponse(id, result);
        }
    }
}
//calculate checksum using XOR of ACSII vals
int SerialComm::calculateChecksum(const String &message) {
    int checksum = 0;
    for (int i = 0; i < message.length(); i++) 
    {
        checksum ^= message[i];
    }
    return checksum;
}

//send response back to sender 
void SerialComm::sendResponse(int id, int data) {
    String responseString = String(id) + ":" + String(data);
    int checksum = calculateChecksum(responseString);
    //format <ID:DATA:CHECKSUM>
    String response = "<" + responseString + ":" + String(checksum) + ">";
    Serial.println(response); // Send the response
}

void SerialComm::requestMissingIDs(int lastReceivedID) 
{
    Serial.println(lastReceivedID);
    String requestString = "<REQ:" + String(lastReceivedID + 1)+">";
    Serial.println(requestString);
}
