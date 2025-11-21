#include "CommsIn.h"
#include <Arduino.h>
#include <SerialAliases.h>
#include <ArduinoJson.h>
#include "CommsOut.h"

// Constructor without ESP ID (defaults to "UNKNOWN")
CommsIn::CommsIn() 
    : espId("UNKNOWN"), commandMap(nullptr), commandMapSize(0), lastReciveTime(0), commsOut(nullptr) {
}

// Constructor with ESP ID
CommsIn::CommsIn(const String& id)
    : espId(id), commandMap(nullptr), commandMapSize(0), lastReciveTime(0), commsOut(nullptr) {
}

// Constructor with ESP ID and command map
CommsIn::CommsIn(const String& id, CommandMap* commands, size_t commandCount)
    : espId(id), commandMap(commands), commandMapSize(commandCount), lastReciveTime(0), commsOut(nullptr) {
}

// Constructor with ESP ID, command map, and CommsOut reference
CommsIn::CommsIn(const String& id, CommandMap* commands, size_t commandCount, CommsOut* commsOut)
    : espId(id), commandMap(commands), commandMapSize(commandCount), lastReciveTime(0), commsOut(commsOut) {
}

// Set command map after construction
void CommsIn::setCommandMap(CommandMap* commands, size_t commandCount) {
    commandMap = commands;
    commandMapSize = commandCount;
}

// Set CommsOut reference for ACK messages
void CommsIn::setCommsOut(CommsOut* commsOut) {
    this->commsOut = commsOut;
}

// Validate that the received ESP ID matches this ESP's ID
bool CommsIn::validateEspId(const String& receivedEspId) {
    return receivedEspId == espId;
}

// Find the handler function for a given command string
CommandHandler CommsIn::findCommandHandler(const String& command) {
    if (commandMap == nullptr || commandMapSize == 0) {
        return nullptr;
    }
    
    for (size_t i = 0; i < commandMapSize; i++) {
        if (command.equals(commandMap[i].commandName)) {
            return commandMap[i].handler;
        }
    }
    
    return nullptr;
}

// Execute a command by finding and calling its handler
void CommsIn::executeCommand(const String& command) {
    CommandHandler handler = findCommandHandler(command);
    
    if (handler != nullptr) {
        bool success = handler();
        
        // Send ACK back to OBC
        if (commsOut != nullptr) {
            commsOut->sendAck(command, success);
        } else {
            // Fallback to direct serial if CommsOut not set
            Serial_1.printf("{\"espId\": \"%s\", \"ack\": %s, \"command\": \"%s\"}\n",
                espId.c_str(), success ? "true" : "false", command.c_str());
        }
        
        if (success) {
            Serial_1.printf("Command executed: %s\n", command.c_str());
        } else {
            Serial_1.printf("Command failed: %s\n", command.c_str());
        }
    } else {
        Serial_1.printf("Error: Unknown command - %s\n", command.c_str());
        // Send ACK for unknown command (failure)
        if (commsOut != nullptr) {
            commsOut->sendAck(command, false);
        } else {
            Serial_1.printf("{\"espId\": \"%s\", \"ack\": false, \"command\": \"%s\"}\n",
                espId.c_str(), command.c_str());
        }
    }
}

void CommsIn::receive() {
    if (Serial_1.available() > 0) {
        String message = Serial_1.readStringUntil('\n');
        message.trim();
        
        if (message.length() > 0) {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, message);

            lastReciveTime = millis();
            
            if (error) {
                Serial_1.print("Error: Incomplete or invalid JSON message - ");
                Serial_1.println(error.c_str());
                return;
            }
            
            // Extract espId and command from JSON
            if (!doc.containsKey("espId") || !doc.containsKey("command")) {
                Serial_1.println("Error: Missing required fields (espId or command)");
                return;
            }
            
            String receivedEspId = doc["espId"].as<String>();
            String command = doc["command"].as<String>();
            
            // Validate ESP ID
            if (!validateEspId(receivedEspId)) {
                Serial_1.printf("Error: ESP ID mismatch. Expected: %s, Received: %s\n", 
                    espId.c_str(), receivedEspId.c_str());
                return;
            }
            
            // Execute the command
            executeCommand(command);
        }
    }
}

// TODO: implement this
bool CommsIn::isStale() {
    if (lastReciveTime > MAX_SILENT_PERIOD_MS) {
        // do something
        return true;
    } else {
        return false;
    }
}
