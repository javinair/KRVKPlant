// MqttClientHandler.cpp
#include "MqttClientHandler.h"


MqttClientHandler::MqttClientHandler(const char* server, uint16_t port, const char* topic, UDPLogger udpLogger)
    : mqttServer(server), mqttPort(port), topic(topic), udpLogger(udpLogger) {}

void MqttClientHandler::setup() {

    mqttClient.onConnect([this](bool sessionPresent) { this->onMqttConnect(sessionPresent); });
    mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason) { this->onMqttDisconnect(reason); });
    mqttClient.onMessage([this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
        this->onMqttMessage(topic, payload, properties, len, index, total);
    });
    mqttClient.setServer(mqttServer, mqttPort);
    connectToMqtt();
}


void MqttClientHandler::connectToMqtt() {
    udpLogger.log(String("Conectando al broker MQTT...").c_str());
    mqttClient.connect();
}

void MqttClientHandler::onMqttConnect(bool sessionPresent) {
    udpLogger.log("Conectado al broker MQTT.");
    mqttClient.subscribe(topic, 1);  // QoS 1
}

void MqttClientHandler::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    udpLogger.log("Desconectado del broker MQTT.");
    connectToMqtt();
}

void MqttClientHandler::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    payload[len] = '\0';    
    udpLogger.log("Mensaje recibido en KRVKPlant:");
        // Llama al callback si está definido
    if (onMessageReceived) {
        onMessageReceived(payload);
    }
}

void MqttClientHandler::publish(const char* topic, const char* payload, uint8_t qos) {
    mqttClient.publish(topic, qos, false, payload);
    udpLogger.log(String("Mensaje publicado: ").c_str());
    udpLogger.log(payload);
    }    

void MqttClientHandler::logMessage(const char* type, const char* message, const char* file, int line, const char* function) {
    JsonDocument doc;

    doc["timestamp"] = TimeHelper::getCurrentTime();  // Fecha y hora
    doc["type"] = type;              // Tipo de mensaje (INFO, ERROR, etc.)
    doc["message"] = message;        // Mensaje

    JsonObject code = doc["code"].to<JsonObject>();
    code["file"] = file;
    code["line"] = line;
    code["function"] = function;

    // Serializar el objeto JSON a una cadena
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    // Publicar el mensaje JSON por MQTT
    publish(MQTT_TOPIC_OUT, jsonBuffer, 0);
}