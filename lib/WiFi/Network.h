#ifndef Network_h
#define Network_h
#include <ESP8266WiFi.h>

class Network {
public:
    Network();
    Network(const char *ssid, const char *password);
    boolean wifi_connect();
    String status();
    void setDeviceAddress();

// protected:
private:
    const char *_ssid;
    const char *_pass;
    IPAddress _device_ip;
    IPAddress _gateway;
    IPAddress _subnet;
    bool _wifi_success = false;
    const short _wifi_connect_timeout = 60;
};

#endif
