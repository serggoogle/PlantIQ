#include <Arduino.h>
#include "Moisture.h"
#define MOISTURE_SENSOR_PIN A2

float Moisture::getMoisture() {
    return analogRead(MOISTURE_SENSOR_PIN);
}
