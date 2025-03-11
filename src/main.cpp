#include <Arduino.h>
#include "secrets.h"

#define LED_PIN 2

unsigned long toggleLedPreviousMillis = 0;
const long toggleLedInterval = 1000;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - toggleLedPreviousMillis >= toggleLedInterval) {
    toggleLedPreviousMillis = currentMillis;

    int ledState = digitalRead(LED_PIN);
    digitalWrite(LED_PIN, !ledState);
  }
}