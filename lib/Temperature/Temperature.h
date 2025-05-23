#ifndef TEMPERATURE_H
#define TEMPERATURE_H

class Temperature {
public:
    Temperature();
    Temperature(int pin);
    static void setup();
    float getTemperature_F();
    float getTemperature_C();
    static bool _temp_sensor_success;

private:
    int _pin;
    OneWire *oneWire;
    DallasTemperature *temp_sensor;
};

#endif