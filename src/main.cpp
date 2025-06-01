#include <Arduino.h>
#include <PrometheusArduino.h>
#include "Plant.h"
#include "Temperature.h"
#include "Network.h"
#include "Prometheus.h"
#include "MQTT.h"

using namespace std;

Plant plant("TestPlant2", "TestSpecies");
Prometheus prometheus;

void setup() {
  delay(3500);
  Serial.begin(9600);
  Serial.println("*********************************************************");

  Serial.println("> Plant Details: ");
  Serial.printf("Plant Name: %s\n", plant.getName().c_str());
  Serial.printf("Plant Species: %s\n", plant.getSpecies().c_str());
  Serial.println("> Instantiating Prometheus:");
  prometheus.setupClients(plant.getName().c_str(), true);
  Serial.println("*********************************************************\n");
}

void loop() {
  /*
   *  Periodically sends data to remote prometheus and mqtt servers
   */
  delay(500);
}
