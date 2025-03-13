#include <Arduino.h>

class ToggleLed {
public:
    ToggleLed(int pin, unsigned long interval);
    void begin();
    void toggle();
    void update();

private:
    int pin;
    bool state;
    unsigned long previousMillis;
    unsigned long interval;    
};

ToggleLed::ToggleLed(int pin, unsigned long interval) : pin(pin), state(false), previousMillis(0), interval(interval) {
    pinMode(pin, OUTPUT);
}

void ToggleLed::begin() {
    digitalWrite(pin, LOW);
}

void ToggleLed::toggle() {
    state = !digitalRead(pin);
    digitalWrite(pin, state);
}

void ToggleLed::update() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        state = !digitalRead(pin);
        digitalWrite(pin, state);
    }
}