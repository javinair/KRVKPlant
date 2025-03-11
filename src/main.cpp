#include <Arduino.h>
#include "secrets.h"

#define LED_PIN 2

void toggleLed();

void toggleLed() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  toggleLed();
}