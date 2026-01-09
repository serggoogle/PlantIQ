package com.plantiq.sensors;

public class TemperatureSensor extends Sensor {
    final double MIN_TEMP_C = 0.0;
    final double MAX_TEMP_C = 41.0;
    public TemperatureSensor(String name) {
        super(name);
        super.setMaxThreshold(MIN_TEMP_C);
        super.setMaxThreshold(MAX_TEMP_C);
    }

}
