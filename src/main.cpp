#include <Arduino.h>
#include <PrometheusArduino.h>
#include "Plant.h"
#include "Temperature.h"
#include "Network.h"
#include "MQTT.h"

using namespace std;


Plant PLANT("TestPlant2", "TestSpecies");

const String HOSTNAME = "ESP-32-" + PLANT.getName();
Network WIFI(HOSTNAME);

void setup() {
  delay(2000);
  Serial.begin(9600);
  Serial.println("*********************************************************");

  Serial.println("> Plant Details: ");
  Serial.printf("Plant Name: %s\n", PLANT.getName().c_str());
  Serial.printf("Plant Species: %s\n", PLANT.getSpecies().c_str());
  Serial.printf("> Instantiating Network: \n");

  if (WIFI.connect()) {
    Serial.printf("Hostname: %s\n", WiFiClass::getHostname());
  }
  else {
    Serial.println("Failed to connect to WiFi network");
  }

  Serial.println("*********************************************************\n");
}

void loop() {
  /*
   *  Periodically sends data to remote prometheus and mqtt servers
   */
  delay(500);
}
