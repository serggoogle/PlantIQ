#include <ArduinoJson.h>
#include "MQTT.h"
#include "MQTT_Config.h"
#include "PubSubClient.h"
#include "WiFiClient.h"
#include "Plant.h"

WiFiClient mqtt_wifi_client = WiFiClient();
PubSubClient client(mqtt_wifi_client);

void MQTT::send_payload(const char* &deviceID, Plant &plant, const char* &sensor, float &value, long long &timestamp) {
    if (!client.connected()) {
        reconnect();
    }
    JsonDocument payload;
    char buffer[512];
    payload["deviceId"] = deviceID;
    payload["plant_name"] = plant.getName();
    payload["plant_type"] = plant.getType();
    payload["plant_species"] = plant.getSpecies();
    payload["sensor_name"] = sensor;
    payload["value"] = value;
    payload["timestamp"] = timestamp;
    // serializeJson(payload, Serial);
    serializeJson(payload, buffer);
    size_t n = serializeJson(payload, buffer);
    char topic[64];
    snprintf(topic, sizeof(topic),
         "esp32/%s/sensor/%s",
         deviceID,
         sensor);
    client.publish(topic, buffer, n);
}

void MQTT::reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void MQTT::connect() {
    Serial.println("********* Setting up MQTT Broker Connection *********");
    Serial.println("> MQTT Broker Config:" );
    Serial.print("> Broker Address: ");
    Serial.println(MQTT_SERVER);
    Serial.print("> Broker Port: ");
    Serial.println(MQTT_PORT);
    Serial.println("> Connecting to MQTT Broker");
    client.setServer(MQTT_SERVER, MQTT_PORT);
    if (!client.connected())
    {
        reconnect();
    }
    Serial.println("> Successfully connected to MQTT Broker");
}

PubSubClient& MQTT::getClient() {
    return client;
}
