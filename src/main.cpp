#include <Arduino.h>
#include <PrometheusArduino.h>
#include "Plant.h"
#include "Network.h"
#include "Adafruit_SHT31.h"
#include "Prometheus.h"
#include "Moisture.h"
#include "MQTT.h"
#include "config.h"

using namespace std;

Adafruit_SHT31 SHT31_SENSOR = Adafruit_SHT31();
Plant PLANT(PLANT_NAME, PLANT_SPECIES);
const char * LABEL = PLANT.getLabel();
Prometheus PROMETHEUS(NUM_SERIES,BUFFER_SIZE);
TimeSeries TEMPERATURE_C_TS(BATCH_SIZE, "temperature_c", LABEL);
TimeSeries TEMPERATURE_F_TS(BATCH_SIZE, "temperature_f", LABEL);
TimeSeries HUMIDITY_TS(BATCH_SIZE, "humidity", LABEL);
TimeSeries MOISTURE_TS(BATCH_SIZE, "moisture", LABEL);
TimeSeries *timeseries[] = {&TEMPERATURE_C_TS, &TEMPERATURE_F_TS, &HUMIDITY_TS, &MOISTURE_TS};

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
    PROMETHEUS.setupClients(PLANT_NAME, DEBUG);
    for (TimeSeries* ts : timeseries) {
        PROMETHEUS.addTimeSeries(*ts);
    }
    Serial.println("*********************************************************\n");
}


void loop() {
    /*
     *  Periodically sends data to remote prometheus and mqtt servers
     */
    float temperature_c, temperature_f, humidity = DEFAULT_SENSOR_VALUE;
    uint16_t moisture = Moisture::getMoisture();
    int64_t time = PROMETHEUS.getTimeMillis();
    string errorMsg = "";

    if (SHT31_SENSOR.readBoth(&temperature_c, &humidity)){
        temperature_f = (temperature_c * 9.0 / 5.0) + 32.0;
    }
    else{
        Serial.println("Error reading temperature & humidity data from sensor.");
        temperature_c = temperature_f = humidity = DEFAULT_SENSOR_VALUE;
    }

    if (moisture >= MAX_MOISTURE_LEVEL || moisture <= MIN_MOISTURE_LEVEL) {
        Serial.println("Error reading moisture data from sensor.");
        moisture = MIN_MOISTURE_LEVEL;
    }

    if (!TEMPERATURE_C_TS.addSample(time, temperature_c) && DEBUG) errorMsg += ("Error adding sample to temperature_c TimeSeries: ") + string(TEMPERATURE_C_TS.errmsg) + '\n';
    if (!TEMPERATURE_F_TS.addSample(time, temperature_f) && DEBUG) errorMsg += ("Error adding sample to temperature_f TimeSeries: ") + string(TEMPERATURE_F_TS.errmsg) + '\n';
    if (!HUMIDITY_TS.addSample(time, humidity) && DEBUG) errorMsg += ("Error adding sample to humidity TimeSeries: ") + string(HUMIDITY_TS.errmsg) + '\n';
    if (!MOISTURE_TS.addSample(time, moisture)) errorMsg += ("Error adding sample to moisture TimeSeries: ") + string(MOISTURE_TS.errmsg) +'\n';
    Serial.print(errorMsg.c_str());

    if (loopCounter >= BATCH_SIZE) {
        //Send after batch is full
        PromClient::SendResult res = PROMETHEUS.client.send(PROMETHEUS.req);
        while (res != PromClient::SendResult::SUCCESS) {
            Serial.printf("Error: Unable to send request(%s)\n", PROMETHEUS.client.errmsg);
            delay(1000);
        }
        // Reset batches after a successful send.
        for (TimeSeries* ts : timeseries) {
            ts->resetSamples();
        }
        loopCounter = 0;
    }
    loopCounter++;
    delay(1000);
}
