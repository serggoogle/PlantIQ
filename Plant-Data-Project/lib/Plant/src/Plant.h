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
        Plant();
        Plant(char * name, char * ssid, char * pass, char * mqtt_broker, int mqtt_port);
        Plant(char * name, char * ssid, char * pass, char * mqtt_broker, int mqtt_port, IPAddress ip_address, IPAddress subnet);
        
        String getName();
        IPAddress getDeviceAddress();

        float getTemperature_F();
        float getTemperature_C();
        int getSoilMoisture();

        void setCustomNetworkFlag();
        void sendPayload(String name, String sensor, float value);
        const char *_ssid;
        const char *_pass;

        void setWiFi();
        void setMQTT();

    private:
        
        String _name;
        char * _mqtt_broker;
        int _mqtt_port;
        
        IPAddress _device_ip;
        IPAddress _gateway;
        IPAddress _subnet;

        bool _wifi_success = false;
        bool _temp_sensor_success = false;
        bool _moist_sensor_success = false;
        bool _usingCustomNetwork = false;

        void setup();
        void setDeviceAddress();
        void setTempSensor();
        void setMoistSensor();
        
        OneWire *oneWire;
        DallasTemperature *temp_sensor;
        
        //Create WiFi & PubSub clients
        WiFiClient *wifiClient;
        PubSubClient *mqttClient;
};

#endif
