#ifndef Prometheus_h
#define Prometheus_h

class Prometheus{
public:
    Prometheus(): req(1,512) {};
    void setupClients(const char* hostname);
    void setupClients(const char* hostname, bool debug);
    void setupClients(const char* hostname, const char* url, char* path, uint16_t port, bool debug);
    WriteRequest req;
};
#endif