#include <Arduino.h>
#include <PromClient.h>
#include <PromLokiTransport.H>
#include "Prometheus.h"
#include "Adafruit_NeoPixel.h"
#include "Config.h"

#define NUM_PIXELS 1
PromLokiTransport transport;
PromClient client(transport);
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void Prometheus::setupClients(const char* hostname){
    setupClients(hostname, URL, PATH, PORT, false);
}

void Prometheus::setupClients(const char* hostname, bool debug){
    setupClients(hostname, URL, PATH, PORT, debug);

}

void Prometheus::setupClients(const char* hostname, const char* url, char* path, uint16_t port, bool debug){
    Serial.println("Setting up client...");
    pixels.begin();
    pixels.setPixelColor(0, pixels.Color(225, 0, 0));
    pixels.show();

    // Configure and start the transport layer
    transport.setUseTls(false);
    transport.setWifiSsid(WIFI_SSID);
    transport.setWifiPass(WIFI_PASSWORD);
    (debug) ? transport.setDebug(Serial) : void();
    if (!transport.begin()) {
        Serial.println(transport.errmsg);
        while (true) {};
    }
    else {
        WiFi.setHostname(hostname);
        Serial.printf("Hostname: %s\n", WiFi.getHostname());
    }

    // Configure and setup prometheus client
    client.setUrl(url);
    client.setPath(path);
    client.setPort(port);
    (debug) ? client.setDebug(Serial) : void();
    if (!client.begin()) {
        Serial.println(client.errmsg);
        while (true) {};
    }

    // Configure Write Request Buffer
    (debug) ? req.setDebug(Serial) : void();

    // Green led indicates a successful connection
    pixels.setPixelColor(0, pixels.Color(0, 225, 0));
    pixels.show();
    Serial.println("Connected");
}