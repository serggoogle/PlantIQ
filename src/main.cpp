#include <Arduino.h>
#include <PrometheusArduino.h>
#include "Plant.h"
#include "Temperature.h"
#include "Network.h"
#include "Prometheus.h"
#include "MQTT.h"

using namespace std;

#define BATCH_SIZE 5
#define NUM_SERIES 2
#define BUFFER_SIZE 512

Plant plant("test-plant-1", "TestSpecies");
String labels = "{job=\"" + plant.getName() + "\",host=\"esp32\"}";
TimeSeries ts1(BATCH_SIZE, "plant_temperature_f", labels.c_str());
TimeSeries ts2(BATCH_SIZE, "plant_moisture", labels.c_str());
Prometheus prometheus(NUM_SERIES,BUFFER_SIZE);

int loopCounter = 0;

void setup() {
    delay(3500);
    Serial.begin(9600);
    Serial.println("*********************************************************");
    Serial.println("> Plant Details: ");
    Serial.printf("Plant Name: %s\n", plant.getName().c_str());
    Serial.printf("Plant Species: %s\n", plant.getSpecies().c_str());
    Serial.println("> Instantiating Prometheus:");
    prometheus.setupClients(plant.getName().c_str(), true);
    prometheus.addTimeSeries(ts1);
    prometheus.addTimeSeries(ts2); // should error out if using default Prometheus constructor with numSeries=1
    Serial.printf("Prometheus Labels: %s\n", labels.c_str());
    Serial.println("*********************************************************\n");
}

void loop() {
    /*
     *  Periodically sends data to remote prometheus and mqtt servers
     */
    int64_t time = prometheus.getTimeMillis();
    float temperature_f = 69;
    float moisture = 500;

    if (loopCounter < BATCH_SIZE) {
        if (!ts1.addSample(time, temperature_f)) {
            Serial.println(ts1.errmsg);
        }
        if (!ts2.addSample(time, moisture)) {
            Serial.println(ts2.errmsg);
        }
        loopCounter++;
    }
    //Send after batch is full
    PromClient::SendResult res = prometheus.client.send(prometheus.req);
    if (!res == PromClient::SendResult::SUCCESS) {
        Serial.printf("Error: Unable to send request(%s)\n", prometheus.client.errmsg);
    }
    // Reset batches after a successful send.
    ts1.resetSamples();
    ts2.resetSamples();
    loopCounter = 0;
    delay(500);
}
