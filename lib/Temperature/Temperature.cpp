#include "Temperature.h"
#include "Arduino.h"
#include <HardwareSerial.h>
#include "DallasTemperature.h"
#include "OneWire.h"

#define ONE_WIRE_BUS 12
#define TEMP_RESOLUTION 12
#define TEMP_SENSOR_INDEX 0

void Temperature::setup()
{
    Serial.println("********* Starting Dallas Temperature Sensor *********");
    // Start up the library
    oneWire = new OneWire(ONE_WIRE_BUS);
    temp_sensor = new DallasTemperature(oneWire);
    temp_sensor->begin();
    temp_sensor->setResolution(TEMP_RESOLUTION);

    // store device information
    const int connectedDevices = temp_sensor->getDeviceCount();
    Serial.println("> Device(s) info:");
    Serial.print("Number of Connected Devices: ");
    Serial.println(connectedDevices);

    if (connectedDevices <= 0)
    {
        _temp_sensor_success = false;
    }
    _temp_sensor_success = true;
}

float Temperature::getTemperature_F(){
    if (&temp_sensor != NULL){
        temp_sensor->requestTemperatures();
        return temp_sensor->getTempFByIndex(TEMP_SENSOR_INDEX);
    }
    return -1;
}

float Temperature::getTemperature_C(){
    temp_sensor->requestTemperatures();
    return temp_sensor->getTempCByIndex(TEMP_SENSOR_INDEX);
}