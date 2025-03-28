# Comms Library

This repository contains the `comms` module used in the Hyperlink Firmware systems. It provides the outbound and inbound communication functionality used to transmit sensor data or receive commands from the OBC.

This library is designed to be modular and interchangeable. It implements the `CommsOutInterface` defined in the shared `interfaces` submodule, allowing it to integrate with systems like `SensorManager` without introducing tight coupling.

## Repository Structure

```
comms/
├── src/
│   ├── CommsOut.h
│   └── CommsOut.cpp
├── Comms-initial-code/          # Archived historical code (not compiled)
├── library.properties
└── README.md
```

- `src/CommsOut.h`: Public interface and class definition for the `CommsOut` class.
- `src/CommsOut.cpp`: Implementation of the `CommsOut` class.
- `Comms-initial-code/`: Legacy experimental files retained for reference. This folder is excluded from builds.
- `library.properties`: Metadata for Arduino CLI and IDE compatibility.

## Purpose

The `CommsOut` class implements a communication backend that allows system components (e.g., `SensorManager`) to push data to the OBC (currently stubbed).
The `CommsIn` class implements a communication backend that allows system components to receive data from the OBC (currently stubbed).

## Design and Interface

This library implements the abstract `CommsOutInterface`, which defines the expected behaviour for all communication output mechanisms.
NB: The `CommsIn` class is not yet implemented but will probably follow the same interface pattern.

### CommsOutInterface (defined in `interfaces/`)

```cpp
class CommsOutInterface {
public:
    virtual void send(const String& sensorName, float value) = 0;
    virtual ~CommsOutInterface() = default;
};
```

### CommsOut Implementation (in this library)

```cpp
class CommsOut : public CommsOutInterface {
public:
    void send(const String& sensorName, float value) override;
};
```

The `send()` method is called by upstream components like `SensorManager` to transmit data.

---

## Usage Example

Used in an Arduino sketch or embedded system:

```cpp
#include <CommsOut.h>
#include <SensorManager.h>

CommsOut comms;
SensorManager manager(&comms);

void setup() {
    Serial.begin(115200);
    manager.setup();
}

void loop() {
    manager.pollSensors();
    delay(1000);
}
```

---

## Dependency

This library depends on the `CommsOutInterface` declared in the `interfaces` submodule. Ensure that the `interfaces` library is available and linked when using `comms`.

NB: The `CommsIn` class is not yet implemented but will probably follow the same interface pattern.

---

## Development Notes

- The library follows Arduino library standards with a `src/` folder and `library.properties` file.
- All logic is encapsulated in the `CommsOut` class.
- The `Comms-initial-code/` folder is retained for legacy reference and excluded from the build process.