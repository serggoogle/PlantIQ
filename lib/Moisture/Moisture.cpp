#include <Arduino.h>
#include "Moisture.h"

#define MOISTURE_SENSOR_PIN A0

int Moisture::getMoisture() {
    return analogRead(MOISTURE_SENSOR_PIN);
}
