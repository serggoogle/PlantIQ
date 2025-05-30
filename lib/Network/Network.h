#ifndef Network_h
#define Network_h
#pragma once
#include <Arduino.h>

class String;
class Network {
public:
    explicit Network(const String &hostname);
    Network(const String &hostname, const char *&ssid, const char *&password);
    [[nodiscard]] bool connect() const;

private:
    String _hostname = "";
    const char *_ssid;
    const char *_pass;
    const short _connection_timeout = 60;
};

#endif
