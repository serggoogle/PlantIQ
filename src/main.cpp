#include <Arduino.h>
#include "Plant.h"
#include "Temperature.h"
#include "Network.h"
#include "MQTT.h"



Plant plant("TestPlant1", "m");

void setup() {
  delay(250);
  Serial.begin(9600);
  Serial.println("*********************************************************\n");
  Serial.println("> Plant Details: ");
  Serial.printf("Plant Name: %s\n", plant.getName().c_str());
  Serial.printf("Plant Species: %s\n", plant.getSpecies().c_str());
  Serial.println("*********************************************************\n");
}

void loop() {
  /*
   *  Periodically sends data to remote prometheus and mqtt servers
   */
  delay(500);
}
