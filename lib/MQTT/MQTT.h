#ifndef MQTT_H
#define MQTT_H
#pragma once
#include <Arduino.h>
#include "Plant.h"
#include "MQTT_Config.h"

class String;
class PubSubClient;

class MQTT {
public:
    static void connect();
    static void send_payload(const char* &deviceID, Plant &plant, const char* &sensor, float &value, long long &timestamp);
    static void reconnect();
    static PubSubClient& getClient();
};

#endif // MQTT_H
