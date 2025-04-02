#include "WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor(int pin, int sampleRate) 
    : _pin(pin), _sampleRate(sampleRate), _lastState(LOW), _callback(nullptr) {}

void WaterLevelSensor::begin() {
    pinMode(_pin, INPUT_PULLUP);
    _lastState = digitalRead(_pin);
    xTaskCreatePinnedToCore(
        sensorTask, "SensorTask", 2048, this, 1, NULL, 1
    );
}

void WaterLevelSensor::setSampleRate(int seconds) {
    _sampleRate = seconds;
}

void WaterLevelSensor::setCallback(CallbackFunction callback) {
    _callback = callback;
}

void WaterLevelSensor::sensorTask(void* parameter) {
    WaterLevelSensor* instance = static_cast<WaterLevelSensor*>(parameter);

    while (true) {
        int currentState = digitalRead(instance->_pin);
        if (currentState != instance->_lastState && instance->_callback) {
            instance->_callback(currentState);
        }
        instance->_lastState = currentState;
        vTaskDelay(instance->_sampleRate * 1000 / portTICK_PERIOD_MS);
    }
}
