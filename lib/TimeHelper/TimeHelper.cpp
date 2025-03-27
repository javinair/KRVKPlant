#include "TimeHelper.h"
#include <WiFi.h>
#include <config.h>  // Para la configuración de NTP

// Inicializamos la variable estática
bool TimeHelper::timeSynced = false;

void TimeHelper::syncTime() {
    const char* ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 3600; // GMT Madrid offset (GMT+1)
    const int daylightOffset_sec = 3600; // Summer time offset (1 hora)

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Error al sincronizar tiempo");
        timeSynced = false;
    } else {
        timeSynced = true;
        Serial.println("Tiempo sincronizado");
    }
}

char* TimeHelper::getCurrentTime() {
    static char timestamp[20];
    
    if (!timeSynced) {
        return "Tiempo no sincronizado";
    }

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    // Formato: DD/MM/YY HH:MM:SS
    strftime(timestamp, sizeof(timestamp), "%d/%m/%y %H:%M:%S", &timeinfo);
    return timestamp;
}
