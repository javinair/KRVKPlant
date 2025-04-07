#include "WaterValve.h"

WaterValve::WaterValve(uint8_t pin, unsigned long maxRunTime, void (*callback)()) 
    : pin(pin), maxRunTime(maxRunTime), startTime(0), isRunning(false), timeoutCallback(callback) {
    pinMode(pin, OUTPUT);
    turnOff();
}

void WaterValve::turnOn() {
    if (!isRunning) {
        startTime = millis();
        isRunning = true;
        digitalWrite(pin, HIGH);
    }
}

void WaterValve::turnOff() {
    digitalWrite(pin, LOW);
    isRunning = false;
    startTime = 0;
}

void WaterValve::toggle() {
    if (isRunning) {
        turnOff();
    } else {
        turnOn();
    }
}

void WaterValve::setState(bool state) {
    if (state) {
        turnOn();
    } else {
        turnOff();
    }
}

void WaterValve::setCallback(void (*callback)()) {
    timeoutCallback = callback;
}

void WaterValve::update() {
    if (isRunning && (millis() - startTime >= maxRunTime)) {
        turnOff();
        if (timeoutCallback) {
            timeoutCallback();
        }
    }
}
