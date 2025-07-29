#include "MQTT.h"
#include "MQTTCredentials.h"
#include "PubSubClient.h"
#include "WiFiClient.h"

MQTT::MQTT(): mqttClient(nullptr) {
    // Utilizing the values from MQTTCredentials.h
    _mqtt_broker = MQTT_SERVER;
    _mqtt_port = MQTT_PORT;
}

MQTT::MQTT(const char* mqtt_server, int mqtt_port): mqttClient(nullptr) {
    _mqtt_broker = mqtt_server;
    _mqtt_port = mqtt_port;
}

void MQTT::send_payload(String name, String sensor, float value)
{
    /*
     * Sends a payload to the MQTT Server
     * TODO:
     * 1. Identify type of payload (json/binary)
     * 2. Add fields to the Plant object.
     * 3. Add Fields of the payload (deviceId, plantName, sensorType, etc)
     *
    */
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
    Serial.println("> Connecting to MQTT Broker");
    // TODO: Select adequate id for mqttClient->connect();
    // mqttClient->connect();

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

