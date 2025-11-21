#ifndef COMMS_IN_H
#define COMMS_IN_H

#include <Arduino.h>
#include <CommsInInterface.h>

// Forward declaration for command handler function type
typedef void (*CommandHandler)();

// Structure to map command strings to handler functions
struct CommandMap {
    const char* commandName;
    CommandHandler handler;
};

class CommsIn : public CommsInInterface {
private:
    unsigned long lastReciveTime;
    static const unsigned long MAX_SILENT_PERIOD_MS = 5000;
    String espId;
    CommandMap* commandMap;
    size_t commandMapSize;

    // Internal methods
    bool validateEspId(const String& receivedEspId);
    void executeCommand(const String& command);
    CommandHandler findCommandHandler(const String& command);

public:
    CommsIn();
    CommsIn(const String& id);
    CommsIn(const String& id, CommandMap* commands, size_t commandCount);
    
    void receive() override;
    bool isStale();
    
    // Method to set command map after construction
    void setCommandMap(CommandMap* commands, size_t commandCount);
};

#endif
