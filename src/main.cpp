#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include "secrets.h"
#include "../lib/MqttClientHandler/MqttClientHandler.h"
#include <time.h>
#include <ESPmDNS.h>
#include "TimeHelper.h"
#include "StatusLED.hpp"
#include "WaterLevelSensor.h"
#include "WaterPump.h"
#include "WaterValve.h"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

/************** Web **************/
AsyncWebServer server(80);

/************** WaterPump **************/
#define WATER_PUMP_PIN 42
#define WATER_PUMP_MAX_RUNTIME 3000 // 3 seconds
WaterPump waterPump(WATER_PUMP_PIN, WATER_PUMP_MAX_RUNTIME);
bool bombaEncendida = false;

/************** WaterValve **************/
#define WATER_VALVE_PIN 41
#define WATER_VALVE_MAX_RUNTIME 3000 // 3 seconds
WaterValve waterValve(WATER_VALVE_PIN, WATER_VALVE_MAX_RUNTIME);


/************** WaterLevelSensor **************/
#define WATER_LEVEL_SENSOR_PIN 34
WaterLevelSensor waterLevelSensor(WATER_LEVEL_SENSOR_PIN, 2);


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
void waterLevelSensorChanged();
void handleWaterPumpTimeout();


/************* IMPLEMENTACIÓN DE MÉTODOS ****************/
void handleMqttMessage(const char* message) {
  udpLogger.log(message);
}

void waterLevelSensorChanged(bool state) {
  if (state == HIGH) {
      LOG_I("Nivel de agua correcto");
  } else {
      LOG_W("Nivel de agua crítico");
  }
  waterPump.setState(state);
  waterValve.setState(state);
  bombaEncendida = state;
}

void handleWaterPumpTimeout() {
  LOG_W("Apagado de bomba de agua por tiempo máximo alcanzado");
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

  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar el sistema de archivos SPIFFS.");
    return;
  }

  // Botones y LED
  // statusLED.setLEDColor(statusLED.getLEDOnColor());
  statusLED.setLEDOff();

  // WaterPump
  waterPump.setCallback(handleWaterPumpTimeout);  

  // WaterLevelSensor
  waterLevelSensor.setCallback(waterLevelSensorChanged);
  waterLevelSensor.begin();

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


  // Servir el archivo index.html cuando accedas a la raíz
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Servir archivos estáticos (HTML, CSS, JS) desde SPIFFS
  // server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/style.css", SPIFFS, "/style.css");
  server.serveStatic("/script.js", SPIFFS, "/script.js");  


  // Ruta para encender la bomba
  server.on("/bomba/encender", HTTP_GET, [](AsyncWebServerRequest *request){
    LOG_I("Bomba encendida");
    bombaEncendida = true;
    request->send(200, "text/plain", "Bomba encendida");
  });

  // Ruta para apagar la bomba
  server.on("/bomba/apagar", HTTP_GET, [](AsyncWebServerRequest *request){
    LOG_I("Bomba apagada");
    bombaEncendida = false;
    request->send(200, "text/plain", "Bomba apagada");
  });

  // Ruta para obtener el estado de la bomba
  server.on("/bomba/estado", HTTP_GET, [](AsyncWebServerRequest *request){
    if (bombaEncendida) {
      request->send(200, "text/plain", "encendida");
    } else {
      request->send(200, "text/plain", "apagada");
    }
  });

  // Iniciar el servidor
  server.begin();  
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
    //TODO
  }

  waterPump.update();
  waterValve.update();
}