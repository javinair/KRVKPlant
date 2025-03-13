// MqttClientHandler.h
#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <AsyncMqttClient.h>
#include "UDPLogger.h"

class MqttClientHandler {
public:
    MqttClientHandler(const char* server, uint16_t port, const char* topic, UDPLogger udpLogger);

    void setup();
    std::function<void(const char*)> onMessageReceived;
    void publish(const char* topic, const char* payload, uint8_t qos = 1);


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

#endif
