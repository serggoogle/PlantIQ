/*
    Temperature.h - Library for interacting with OneWire.h and DallasTemperature.h
    Created by Sergio Olvera, May 23, 2025
*/

#ifndef Temperature_h
#define Temperature_h

class OneWire;
class DallasTemperature;

class Temperature {
public:
    Temperature(): _pin(0){
    };
    static void setup();
    static float getTemperature_F();
    static float getTemperature_C();
    static bool _temp_sensor_success;

private:
    int _pin;
    static OneWire *oneWire;
    static DallasTemperature *temp_sensor;

};

#endif