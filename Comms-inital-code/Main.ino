#include "SerialComm.h"

//instantiate 
SerialComm serialComm;

void setup() 
{
    Serial.begin(115200);  //initialize at 115200 baud
    delay(2000);           
    serialComm.setupTasks();  //FreeRTOS task setup for dual-core functionality
}

//modify later 
void loop() 
{
    //no processing here as tasks run in parallel
    vTaskDelay(pdMS_TO_TICKS(1000));  
}
