#ifndef Prometheus_h
#define Prometheus_h

class Prometheus{
public:
    Prometheus(): req(1, 512) {};
    Prometheus(uint32_t numSeries, uint32_t bufferSize): req(numSeries, bufferSize) {};
    void setupClients(const char* hostname);
    void setupClients(const char* hostname, bool debug);
    void setupClients(const char* hostname, const char* url, char* path, uint16_t port, bool debug);
    void addTimeSeries(TimeSeries& series);
    int64_t getTimeMillis();
    PromClient client;
    WriteRequest req;
private:
    PromLokiTransport transport;
};
#endif