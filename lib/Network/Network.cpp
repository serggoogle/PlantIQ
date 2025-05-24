#include "Arduino.h"
#include "Network.h"
#include "WiFiCredentials.h"

#define ON_BOARD_LED 2

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

    // Using Built-In LED
    pinMode(LED_BUILTIN, OUTPUT);

    while (WiFi.status() != WL_CONNECTED) {
        // While attempting to connect, on-board led will keep blinking
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Arduino successfully connected to WiFi Network");
        WiFi.setHostname(_hostname.c_str());
        // Solid LED indicates a successful connection
        digitalWrite(LED_BUILTIN, LOW);
        Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
        return true;
    }
    return false;
}
