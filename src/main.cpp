#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include "secrets.h"
#include "../lib/MqttClientHandler/MqttClientHandler.h"
#include <time.h>
#include <ESPmDNS.h>
#include "TimeHelper.h"
#include "StatusLED.hpp"

/************** Status LED **************/
#define LED_PIN 18
StatusLED statusLED(LED_PIN);

/************* OTA ****************/
#define OTA_UPDATE_MS 1000

/************* UDPLogger ****************/
UDPLogger udpLogger(BROADCAST_IP, BROADCAST_PORT);

/************* MQTT ****************/
int secondsToSendMQTT = 2000;
unsigned long previousMQTTMillis = 0;
MqttClientHandler mqttClientHandler(MQTT_SERVER_IP, MQTT_SERVER_PORT, MQTT_TOPIC_IN, udpLogger);

/************* DECLARACIÓN DE MÉTODOS ****************/
void handleMqttMessage(const char* message);


/************* IMPLEMENTACIÓN DE MÉTODOS ****************/
void handleMqttMessage(const char* message) {
  udpLogger.log(message);
}

void otaConfiguration() {
  ArduinoOTA.setHostname(HOST_NAME);
  ArduinoOTA.onStart([]() {
      Serial.println("Inicio de actualización OTA...");
  });
  ArduinoOTA.onEnd([]() {
      Serial.println("Actualización OTA completada.");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});
  ArduinoOTA.onError([](ota_error_t error) {
      char buffer[50];
      snprintf(buffer, sizeof(buffer), "Error OTA: %u", error);
      Serial.println(buffer);
  });
  ArduinoOTA.begin();
}

void setup() {
  // Botones y LED
  // statusLED.setLEDColor(statusLED.getLEDOnColor());

  // WiFi
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
  }
  otaConfiguration();  

  // Serial
  Serial.begin(115200);
  Serial.println("Conectado");
  Serial.println(WiFi.localIP());

  // mDNS
  if (MDNS.begin(HOST_NAME)) {  // HOST_NAME.local
    Serial.println("mDNS iniciado con éxito");
  } else {
    Serial.println("Error al iniciar mDNS");
  }

  // Sincronización
  TimeHelper::syncTime();

  // Mensajería
  udpLogger.init();
  mqttClientHandler.setup();
  mqttClientHandler.onMessageReceived = handleMqttMessage;
}

void loop() {
  unsigned long currentMillis = millis();

  // OTA
  static unsigned long lastOTACheck = 0;    
  if (currentMillis - lastOTACheck >= OTA_UPDATE_MS) {
      lastOTACheck = currentMillis;
      ArduinoOTA.handle();
  }

  if (currentMillis - previousMQTTMillis >= secondsToSendMQTT) {
    previousMQTTMillis = currentMillis;
    LOG_I("MQTT Test");
  }
}