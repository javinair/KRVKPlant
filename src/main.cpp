#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"
#include "../lib/MqttClientHandler/MqttClientHandler.h"

#define LED_PIN 2

unsigned long toggleLedPreviousMillis = 0;
const long toggleLedInterval = 2000;

UDPLogger udpLogger(BROADCAST_IP, BROADCAST_PORT);
MqttClientHandler mqttClientHandler(MQTT_SERVER_IP, MQTT_SERVER_PORT, MQTT_TOPIC_IN, udpLogger);

void handleMqttMessage(const char* message);

void handleMqttMessage(const char* message) {
  udpLogger.log(message);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
  }

  udpLogger.init();
    
  mqttClientHandler.setup();
  mqttClientHandler.onMessageReceived = handleMqttMessage;
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - toggleLedPreviousMillis >= toggleLedInterval) {
    toggleLedPreviousMillis = currentMillis;

    int ledState = digitalRead(LED_PIN);
    digitalWrite(LED_PIN, !ledState);
    mqttClientHandler.publish(MQTT_TOPIC_OUT, ledState == HIGH ? "ON" : "OFF");
  }
}