#ifndef Network_h
#define Network_h
#include <ESP8266WiFi.h>

class Network {
public:
    explicit Network(const String &hostname);
    Network(const String &hostname, const char *&ssid, const char *&password);
    [[nodiscard]] boolean connect() const;

private:
    String _hostname = "ESP-12";
    const char *_ssid;
    const char *_pass;
    const short _wifi_connect_timeout = 60;
};

#endif
