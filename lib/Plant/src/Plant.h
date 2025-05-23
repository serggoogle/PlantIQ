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
        Plant(char * name);
        Plant(char * name, char * species);
        
        String getName();
        String getSpecies();

        int getSoilMoisture();

        void setCustomNetworkFlag();
        void sendPayload(String name, String sensor, float value);
        void setMQTT();

    private:
        String _name;
        String _species;
        
        bool _moist_sensor_success = false;
        bool _usingCustomNetwork = false;

        void setup();
        void setDeviceAddress();
        void setMoistSensor();
        
};

#endif
