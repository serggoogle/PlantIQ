#include <Arduino.h>
#include "Moisture.h"

#define MOISTURE_SENSOR_PIN A0

uint16_t Moisture::getMoisture() {
    return analogRead(MOISTURE_SENSOR_PIN);
}
