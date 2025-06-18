#include <Arduino.h>
#include <PromClient.h>
#include <PromLokiTransport.h>
#include "Prometheus.h"
#include "Config.h"


void Prometheus::setupClients(const char* hostname){
    setupClients(hostname, URL, PATH, PORT, false);
}

void Prometheus::setupClients(const char* hostname, bool debug){
    setupClients(hostname, URL, PATH, PORT, debug);
}

void Prometheus::setupClients(const char* hostname, const char* url, char* path, uint16_t port, bool debug){
    client.setTransport(transport);
    Serial.println("Setting up client...");

    // Configure and start the transport layer
    transport.setUseTls(false);
    transport.setWifiSsid(WIFI_SSID);
    transport.setWifiPass(WIFI_PASSWORD);
    (debug) ? transport.setDebug(Serial) : void();
    if (!transport.begin()) {
        Serial.println(transport.errmsg);
        while (true) {};
    }

    WiFi.setHostname(hostname);
    Serial.printf("Hostname: %s\n", WiFi.getHostname());

    // Configure and setup prometheus client
    client.setUrl(url);
    client.setPath(path);
    client.setPort(port);
    (debug) ? client.setDebug(Serial) : void();
    if (!client.begin()) {
        Serial.println(client.errmsg);
        while (true) {};
    }

    Serial.println("Connected");
}

void Prometheus::addTimeSeries(TimeSeries &series) {
    Serial.println("Adding time-series...");
    if (!req.addTimeSeries(series)) {
        Serial.println(req.errmsg);
    }
}

int64_t Prometheus::getTimeMillis() {
    return transport.getTimeMillis();
}