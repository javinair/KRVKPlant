#include "UDPLogger.h"

UDPLogger::UDPLogger(const char* remoteIPString, uint16_t remotePort) {
    this->remoteIP = parseIPAddress(remoteIPString);
    this->remotePort = remotePort;
}

void UDPLogger::init() {
}

void UDPLogger::log(const char *message) {
    udp.beginPacket(remoteIP, remotePort);
    udp.write((const uint8_t*)message, strlen(message));
    udp.endPacket();
}

IPAddress UDPLogger::parseIPAddress(const char* ip) {
    uint8_t octets[4] = {0};
    sscanf(ip, "%hhu.%hhu.%hhu.%hhu", &octets[0], &octets[1], &octets[2], &octets[3]);
    return IPAddress(octets[0], octets[1], octets[2], octets[3]);
}