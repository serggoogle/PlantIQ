#include <Arduino.h>
#include <PrometheusArduino.h>
#include "Plant.h"
#include "Network.h"
#include "Adafruit_SHT31.h"
#include "Prometheus.h"
#include "Moisture.h"
#include "MQTT.h"

using namespace std;

constexpr int8_t SHT31_ADDR = 0x44; // Set to 0x45 for alternate i2c addr
constexpr size_t BATCH_SIZE = 5;
constexpr size_t NUM_SERIES = 4;
constexpr size_t BUFFER_SIZE = 1024;
constexpr const char* PLANT_NAME = "test-plant-1";
constexpr const char* PLANT_SPECIES = "TestSpecies";
constexpr float DEFAULT_SENSOR_VALUE = 0.0f;
constexpr bool DEBUG = true;

Adafruit_SHT31 SHT31_SENSOR = Adafruit_SHT31();
Plant PLANT(PLANT_NAME, PLANT_SPECIES);
const char * LABEL = PLANT.getLabel();
Prometheus PROMETHEUS(NUM_SERIES,BUFFER_SIZE);
TimeSeries TEMPERATURE_C_TS(BATCH_SIZE, "temperature_c", LABEL);
TimeSeries TEMPERATURE_F_TS(BATCH_SIZE, "temperature_f", LABEL);
TimeSeries HUMIDITY_TS(BATCH_SIZE, "humidity", LABEL);
TimeSeries MOISTURE_TS(BATCH_SIZE, "moisture", LABEL);


int loopCounter = 0;
bool enableHeater = false;

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        delay(10); 
    }
    Serial.println("*********************************************************");
    Serial.println("> Plant Details: ");
    Serial.printf("Plant Name: %s\n", PLANT.getName());
    Serial.printf("Plant Species: %s\n", PLANT.getSpecies());

    Serial.println("> Initializing SHT31 Temperature/Moisture Sensor:");
    if (!SHT31_SENSOR.begin(SHT31_ADDR)) {
        Serial.println("Couldn't find SHT31");
        while(1) delay(1);
    }

    Serial.println("> Instantiating Prometheus:");
    Serial.printf("Prometheus Labels: %s\n",LABEL);
    PROMETHEUS.setupClients(PLANT.getName(), DEBUG);
    PROMETHEUS.addTimeSeries(TEMPERATURE_C_TS);
    PROMETHEUS.addTimeSeries(TEMPERATURE_F_TS);
    PROMETHEUS.addTimeSeries(HUMIDITY_TS);
    PROMETHEUS.addTimeSeries(MOISTURE_TS);
    Serial.println("*********************************************************\n");
}


void loop() {
    /*
     *  Periodically sends data to remote prometheus and mqtt servers
     */
    float temperature_c, temperature_f, humidity = DEFAULT_SENSOR_VALUE;
    uint16_t moisture = Moisture::getMoisture();
    int64_t time = PROMETHEUS.getTimeMillis();
    if (SHT31_SENSOR.readBoth(&temperature_c, &humidity)){
        temperature_f = (temperature_c * 9.0 / 5.0) + 32.0;
        if (!TEMPERATURE_C_TS.addSample(time, temperature_c)) {
            Serial.println(TEMPERATURE_C_TS.errmsg);
        }
        if (!TEMPERATURE_F_TS.addSample(time, temperature_f)) {
            Serial.println(TEMPERATURE_F_TS.errmsg);
        }
        if(!HUMIDITY_TS.addSample(time, humidity)){
            Serial.println(HUMIDITY_TS.errmsg);
        }
    }
    else{
        Serial.println("Error reading temperature & humidity");
    }
    if (moisture < 0) {
        Serial.println("Error reading moisture");
        moisture = DEFAULT_SENSOR_VALUE; // Set to default value on error
    } else {
        Serial.printf("Moisture: %d\n", moisture);
        if (!MOISTURE_TS.addSample(time, moisture)) {
            Serial.println(MOISTURE_TS.errmsg);
        }
    }

    if (loopCounter >= BATCH_SIZE) {
        //Send after batch is full
        PromClient::SendResult res = PROMETHEUS.client.send(PROMETHEUS.req);
        if (!res == PromClient::SendResult::SUCCESS) {
            Serial.printf("Error: Unable to send request(%s)\n", PROMETHEUS.client.errmsg);
        }
        // Reset batches after a successful send.
        TEMPERATURE_C_TS.resetSamples();
        TEMPERATURE_F_TS.resetSamples();
        HUMIDITY_TS.resetSamples();
        MOISTURE_TS.resetSamples();
        loopCounter = 0;
    }
    loopCounter++;
    delay(1000);
}
