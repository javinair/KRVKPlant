#ifndef FLOAT_SENSOR_H
#define FLOAT_SENSOR_H

#include <Arduino.h>

class WaterLevelSensor {
public:
    using CallbackFunction = void (*)(bool state);

    WaterLevelSensor(int pin, int sampleRate = 1);
    void begin();
    void setSampleRate(int seconds);
    void setCallback(CallbackFunction callback);

private:
    int _pin;
    int _sampleRate;
    int _lastState;
    CallbackFunction _callback;
    
    static void sensorTask(void* parameter);
};

#endif
