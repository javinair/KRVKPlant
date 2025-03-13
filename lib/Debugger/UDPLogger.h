#ifndef UDPLOGGER_H
#define UDPLOGGER_H

#include <WiFiUdp.h>
#include <Arduino.h>

class UDPLogger {
public:
    UDPLogger(const char *remoteIPString, uint16_t remotePort);
    void init();
    void log(const char *message);

private:
    WiFiUDP udp;
    IPAddress remoteIP;
    uint16_t remotePort;
    static IPAddress parseIPAddress(const char* ip);
};

#endif