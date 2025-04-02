#include "WaterPump.h"

WaterPump::WaterPump(uint8_t pin, unsigned long maxRunTime, void (*callback)()) 
    : pin(pin), maxRunTime(maxRunTime), startTime(0), isRunning(false), timeoutCallback(callback) {
    pinMode(pin, OUTPUT);
    turnOff();
}

void WaterPump::turnOn() {
    if (!isRunning) {
        startTime = millis();
        isRunning = true;
        digitalWrite(pin, HIGH);
    }
}

void WaterPump::turnOff() {
    digitalWrite(pin, LOW);
    isRunning = false;
    startTime = 0;
}

void WaterPump::toggle() {
    if (isRunning) {
        turnOff();
    } else {
        turnOn();
    }
}

void WaterPump::setState(bool state) {
    if (state) {
        turnOn();
    } else {
        turnOff();
    }
}

void WaterPump::setCallback(void (*callback)()) {
    timeoutCallback = callback;
}

void WaterPump::update() {
    if (isRunning && (millis() - startTime >= maxRunTime)) {
        turnOff();
        if (timeoutCallback) {
            timeoutCallback();
        }
    }
}
