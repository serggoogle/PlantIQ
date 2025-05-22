#include "Arduino.h"
#include "Network.h"
#include <WiFiCredentials.h>


Network::Network(){
    // Utilizing the variables from WiFiCredentials.h
    _ssid = SSID;
    _pass = PASSWORD;
}

Network::Network(const char *ssid, const char *password){
    _ssid = ssid;
    _pass = password;
}

void Network::setDeviceAddress() {
    _device_ip = WiFi.localIP();
}

boolean Network::wifi_connect() {
    Serial.println("********* Connecting to ");
    Serial.print(_ssid);
    Serial.println(" network *********");

    WiFi.begin(_ssid, _pass);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println(".");
        delay(1000);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("> Arduino connected to Wifi");
        setDeviceAddress();
        _wifi_success = true;
        return true;
    }
    return false;
}
