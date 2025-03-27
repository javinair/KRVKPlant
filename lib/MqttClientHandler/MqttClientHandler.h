// MqttClientHandler.h
#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <AsyncMqttClient.h>
#include "UDPLogger.h"
#include <ArduinoJson.h>
#include <../../src/secrets.h>
#include <../TimeHelper/TimeHelper.h>

class MqttClientHandler {
public:
    MqttClientHandler(const char* server, uint16_t port, const char* topic, UDPLogger udpLogger);

    void setup();
    std::function<void(const char*)> onMessageReceived;
    void publish(const char* topic, const char* payload, uint8_t qos = 1);
    void logMessage(const char* type, const char* message, const char* file, int line, const char* function);


private:
    AsyncMqttClient mqttClient;
    const char* mqttServer;
    uint16_t mqttPort;
    const char* topic;
    UDPLogger udpLogger;    

    void connectToMqtt();

    // Callbacks
    void onMqttConnect(bool sessionPresent);
    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
};

#define LOG_MESSAGE(type, message) mqttClientHandler.logMessage(type, message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_D(message) mqttClientHandler.logMessage( "🟢", message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_I(message) mqttClientHandler.logMessage( "🔵", message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_W(message) mqttClientHandler.logMessage( "🟡", message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_E(message) mqttClientHandler.logMessage( "🔴", message, __FILE__, __LINE__, __FUNCTION__)

#endif
