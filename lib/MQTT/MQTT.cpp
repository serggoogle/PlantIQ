#include "MQTT.h"
#include "Network.h"
#include "MQTTCredentials.h"
#include "PubSubClient.h"

MQTT::MQTT() {
    // Utilizing the values from MQTTConfig.h
    _mqtt_broker = MQTT_SERVER;
    _mqtt_port = MQTT_PORT;
}

MQTT::MQTT(const char* mqtt_server, int mqtt_port) {
    _mqtt_broker = mqtt_server;
    _mqtt_port = mqtt_port;
}

void MQTT::mqtt_connect() {
    Serial.println("********* Setting up MQTT Broker Connection *********");
    Serial.println("> Creating new WiFi Client (for MQTT Server)");
    WiFiClient mqtt_wifi_client = WiFiClient();
    mqttClient = new PubSubClient(mqtt_wifi_client);

    Serial.println("> MQTT Broker Config:");
    Serial.print("> Broker IP: ");
    Serial.println(_mqtt_broker);
    Serial.print("> Broker Port: ");
    Serial.println(_mqtt_port);

    mqttClient->setServer(_mqtt_broker, _mqtt_port);
    mqttClient->connect(_name.c_str());
    Serial.println("> Connecting to MQTT Broker");

    int RECONNECT_COUNT = 25;
    while (!mqttClient->connected() && RECONNECT_COUNT > 0)
    {
        Serial.println(".");
        delay(1000);
        RECONNECT_COUNT--;
    }

    if (RECONNECT_COUNT == 0)
    {
        Serial.println("> Unable to connect to MQTT Broker");
    }
    else
    {
        Serial.println("> Successfully connected to MQTT Broker");
    }
}

