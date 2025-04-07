#ifndef WATER_VALVE_H
#define WATER_VALVE_H

#include <Arduino.h>

class WaterValve {
private:
    uint8_t pin;
    unsigned long maxRunTime;
    unsigned long startTime;
    bool isRunning;
    void (*timeoutCallback)();

public:
    WaterValve(uint8_t pin, unsigned long maxRunTime, void (*callback)() = nullptr);
    void turnOn();
    void turnOff();
    void toggle();
    void setState(bool state);
    void update();
    void setCallback(void (*callback)());
};

#endif // WATER_VALVE_H
