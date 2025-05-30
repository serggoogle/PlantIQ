#include <WiFi.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Network.h"
#include "WiFiCredentials.h"

#define NUM_PIXELS 1      // We only have 1 NeoPixel

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

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

    pixels.begin();
    pixels.show();

    while (WiFiClass::status() != WL_CONNECTED) {
        Serial.print(".");
        // TODO: Utilize the _connection_timeout so it doesn't keep attempting indefinitely
        // While attempting to connect, on-board led will keep blinking red
        pixels.setPixelColor(0, pixels.Color(255, 0, 0));
        pixels.show(); // Send the updated pixel data to the display
        delay(500);
        pixels.clear();
        pixels.show();
        delay(500);
    }

    if (WiFiClass::status() == WL_CONNECTED) {
        Serial.println("\nArduino successfully connected to WiFi Network");
        WiFiClass::setHostname(_hostname.c_str());
        // Solid green led indicates a successful connection
        pixels.setPixelColor(0, pixels.Color(0, 225, 0));
        pixels.show();
        Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
        return true;
    }
    return false;
}
