package com.plantiq.sensors;

public class HumiditySensor extends Sensor {
    final double MIN_HUMIDITY_LEVEL = 0.0;
    final double MAX_HUMIDITY_LEVEL = 50.0;
    public HumiditySensor(String name, String deviceId){
        super(name, deviceId);
        super.setMaxThreshold(MIN_HUMIDITY_LEVEL);
        super.setMaxThreshold(MAX_HUMIDITY_LEVEL);
    }
}
