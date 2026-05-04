#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "time.h"
#include "Plant.h"
#include "Adafruit_SHT31.h"
#include "Moisture.h"
#include "MQTT.h"
#include "Sensor_Config.h"
#include "Network_Config.h"
#include "PubSubClient.h"
using namespace std;

Adafruit_SHT31 SHT31_SENSOR = Adafruit_SHT31();
Plant PLANT(PLANT_NAME, PLANT_TYPE, PLANT_SPECIES);

int loopCounter = 0;
bool enableHeater = false;

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        delay(10); 
    }
    Serial.println("*********************************************************");
    Serial.println("> Connecting to WiFi Network: ");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1);
    }
    configTime(0, 0, NTP_SERVER); // UTC
    Serial.println("*********************************************************");
    Serial.println("> Plant Details: ");
    Serial.printf("Plant Name: %s\n", PLANT.getName());
    Serial.printf("Plant Species: %s\n", PLANT.getSpecies());
    
    Serial.println("> Initializing SHT31 Temperature/Moisture Sensor: ");
    if (!SHT31_SENSOR.begin(SHT31_ADDR)) {
        Serial.println("Couldn't find SHT31");
        while(1) delay(1);
    }
     // Set keep alive to be slightly longer than publish interval to avoid disconnects
    MQTT::getClient().setKeepAlive(PUBLISH_INTERVAL_MS + 5000);
    MQTT::connect();
}

long long getTimestampMillis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (long long) tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

void loop() {
    /*
     *  Periodically sends data to remote mqtt servers
     */
    float temperature_c, temperature_f, humidity = DEFAULT_SENSOR_VALUE;
    float moisture = Moisture::getMoisture();
    if (SHT31_SENSOR.readBoth(&temperature_c, &humidity)){
        temperature_f = (temperature_c * 9.0 / 5.0) + 32.0;
    }
    else{
        temperature_c = temperature_f = humidity = DEFAULT_SENSOR_VALUE;
    }
    long long timestamp = getTimestampMillis();
    MQTT::send_payload(DEVICE_ID, PLANT, TEMPERATURE_C_SENSOR_NAME, temperature_c, timestamp);
    MQTT::send_payload(DEVICE_ID, PLANT, TEMPERATURE_F_SENSOR_NAME, temperature_f, timestamp);
    MQTT::send_payload(DEVICE_ID, PLANT, HUMIDITY_SENSOR_NAME, humidity, timestamp);
    MQTT::send_payload(DEVICE_ID, PLANT, MOISTURE_SENSOR_NAME, moisture, timestamp);
    MQTT::getClient().loop();
    delay(PUBLISH_INTERVAL_MS);
}
