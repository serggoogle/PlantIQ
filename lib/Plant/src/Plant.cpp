#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "DallasTemperature.h"
#include "ArduinoJson.h"
#include "OneWire.h"
#include "Plant.h"

#define ONE_WIRE_BUS 12
#define TEMP_RESOLUTION 12
#define TEMP_SENSOR_INDEX 0
#define MOISTURE_SENSOR_PIN A0

Plant::Plant(char *name)
{
    _name = name;
}

Plant::Plant(char *name, char* species)
{
    _name = name;
    _species = species;
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

String Plant::getName()
{
    return _name;
}
// Temperature Code
// void Plant::setTempSensor()
// {
//     Serial.println("********* Starting Dallas Temperature Sensor *********");
//     // Start up the library
//     oneWire = new OneWire(ONE_WIRE_BUS);
//     temp_sensor = new DallasTemperature(oneWire);
//     temp_sensor->begin();
//     temp_sensor->setResolution(TEMP_RESOLUTION);
//
//     // store device information
//     const int connectedDevices = temp_sensor->getDeviceCount();
//     Serial.println("> Device(s) info:");
//     Serial.print("Number of Connected Devices: ");
//     Serial.println(connectedDevices);
//
//     if (connectedDevices > 0)
//     {
//         _temp_sensor_success = true;
//     }
// }
// float Plant::getTemperature_F()
// {
//     if (&temp_sensor != NULL){
//         temp_sensor->requestTemperatures(); // Send the command to get temperatures
//         return temp_sensor->getTempFByIndex(TEMP_SENSOR_INDEX);
//     }
//     return -1;
// }
//
// float Plant::getTemperature_C()
// {
//     temp_sensor->requestTemperatures(); // Send the command to get temperatures
//     return temp_sensor->getTempCByIndex(TEMP_SENSOR_INDEX);
// }

// Moisture Code
// int Plant::getSoilMoisture()
// {
//     return analogRead(MOISTURE_SENSOR_PIN);
// }