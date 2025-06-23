#include <WiFi.h>
#include <Arduino.h>
#include "Network.h"
#include "WiFiCredentials.h"

Network::Network(const String &hostname){
    // Utilizing the variables from WiFiCredentials.h
    _hostname = hostname;
    _ssid = SSID;
    _pass = PASSWORD;
}

Network::Network(const String &hostname, const char *&ssid, const char *&password){
    _hostname = hostname;
    _ssid = ssid;
    _pass = password;
}

boolean Network::connect() const {
    WiFi.begin(_ssid, _pass);

    while (WiFiClass::status() != WL_CONNECTED) {
        Serial.print(".");
        // TODO: Utilize the _connection_timeout so it doesn't keep attempting indefinitely
        delay(500);
    }

    if (WiFiClass::status() == WL_CONNECTED) {
        Serial.println("\nArduino successfully connected to WiFi Network");
        WiFiClass::setHostname(_hostname.c_str());
        Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
        return true;
    }
    return false;
}
