#include <PubSubClient.h>
#ifndef MQTT_h
#define MQTT_h

class MQTT {
public:
    MQTT();
    MQTT(const char* mqtt_server, int mqtt_port);
    void mqtt_connect();
private:
    const char* _mqtt_broker;
    int _mqtt_port;
    PubSubClient *mqttClient;
};

#endif
