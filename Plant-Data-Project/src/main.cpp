#include <Arduino.h>
#include <Plant.h>

Plant testPlant("TestPlant2");
char mqtt_broker[] = "*MQTT-IP*";
int mqtt_port = 1883;

void setup() {
  // put your setup code here, to run once:
    // start serial port
  Serial.begin(9600);

  // testPlant = Plant(plantName, mqtt_broker, mqtt_port);
  // testPlant.setWiFi();

  Serial.println("> Creating plant:"); 
  Serial.println("> Plant Details: ");
  Serial.printf("Name: %s\n", testPlant.getName().c_str());
  // Serial.printf("Address: %s\n", testPlant.getDeviceAddress().toString().c_str());
}

void loop() {
  // put your main code here, to run repeatedly:
  // float temp_f = testPlant.getTemperature_F();
  int moisture = testPlant.getSoilMoisture();
  
  Serial.print("Moisture: ");
  Serial.println(moisture);

  // testPlant.sendPayload(plantName,"/temperature", temp_f);
  // testPlant.sendPayload(plantName,"/moisture", moisture);
  delay(500);
}