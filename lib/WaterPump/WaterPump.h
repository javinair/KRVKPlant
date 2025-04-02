#ifndef WATER_PUMP_H
#define WATER_PUMP_H

#include <Arduino.h>

class WaterPump {
private:
    uint8_t pin;
    unsigned long maxRunTime;
    unsigned long startTime;
    bool isRunning;
    void (*timeoutCallback)();

public:
    WaterPump(uint8_t pin, unsigned long maxRunTime, void (*callback)() = nullptr);
    void turnOn();
    void turnOff();
    void toggle();
    void setState(bool state);
    void update();
    void setCallback(void (*callback)());
};

#endif // WATER_PUMP_H
