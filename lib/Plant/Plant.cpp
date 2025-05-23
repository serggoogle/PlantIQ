#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "DallasTemperature.h"
#include "ArduinoJson.h"
#include "OneWire.h"
#include "Plant.h"
#define MOISTURE_SENSOR_PIN A0

Plant::Plant(const String &name)
{
    _name = name;
}

Plant::Plant(const String &name, const String &species)
{
    _name = name;
    _species = species;
}

String Plant::getName()
{
    return _name;
}

String Plant::getSpecies() {
    return _species;
}

// Main Arduino Code
// void Plant::setup()
// {
//     Serial.println("*********************************************************\n");
//     setWiFi();
//     setTempSensor();
//     setMQTT();
//     // setMoistSensor();
//     if (!_wifi_success)
//     {
//         Serial.println("> Error: Unable To connect to network.");
//         Serial.println(WiFi.status());
//         // setWiFi();
//     }
//     if (!_temp_sensor_success)
//     {
//         Serial.println("> Error: No temperature sensor(s) found");
//     }
//     Serial.println("*********************************************************\n");
// }

// Moisture Code
// int Plant::getSoilMoisture()
// {
//     return analogRead(MOISTURE_SENSOR_PIN);
// }