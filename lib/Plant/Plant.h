/*
    Temperature.h - Library for interacting with OneWire.h and DallasTemperature.h
    Created by Sergio Olvrera, June 15, 2024
*/
#ifndef Plant_h
#define Plant_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DallasTemperature.h>

// Class definition
class Plant {
public:
    explicit Plant(const String &name);
    Plant(const String &name, const String &species);
    String getName();
    String getSpecies();

private:
    String _name;
    String _species;

    // void setup();
    // void setDeviceAddress();
    // int getSoilMoisture();
    // void setMoistSensor();
    // bool _moist_sensor_success = false;
};

#endif
