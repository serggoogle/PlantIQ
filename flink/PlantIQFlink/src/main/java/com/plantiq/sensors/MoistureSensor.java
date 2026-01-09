package com.plantiq.sensors;

public class MoistureSensor extends Sensor {
    final double MIN_MOISTURE_LEVEL = 0.0;
    final double MAX_MOISTURE_LEVEL = 3800;
    public MoistureSensor(String name){
        super(name);
        super.setMaxThreshold(MIN_MOISTURE_LEVEL);
        super.setMaxThreshold(MAX_MOISTURE_LEVEL);
    }
}
