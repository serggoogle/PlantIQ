package com.plantiq.sensors;

public class TemperatureSensor extends Sensor {
    private static double celsiusToFahrenheit(double temperature) {
        return (temperature * 9 / 5) + 32;
    }

    final double MIN_TEMP_C = 0.0;
    final double MAX_TEMP_C = 41.0;

    public TemperatureSensor(String deviceId, String plantName, String sensorName, Boolean isImperial) {
        super(deviceId, plantName, sensorName);
        if (isImperial){
            super.setMinThreshold(celsiusToFahrenheit(MIN_TEMP_C));
            super.setMaxThreshold(celsiusToFahrenheit(MAX_TEMP_C));
        }
        else {
            super.setMaxThreshold(MIN_TEMP_C);
            super.setMaxThreshold(MAX_TEMP_C);
        }
    }
}
