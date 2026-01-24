package com.plantiq.sensors;

import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.annotation.JsonCreator;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.annotation.JsonIgnore;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.annotation.JsonProperty;

import java.io.Serializable;

public class Sensor implements Serializable {
    private String deviceId = "";
    private String name = "";
    private double minThreshold = 0.0;
    private double maxThreshold = 0.0;
    private double value = 0.0;

    @JsonCreator(mode = JsonCreator.Mode.PROPERTIES)
    public Sensor(@JsonProperty("sensor") String name, @JsonProperty("deviceId") String deviceId) {
        this.name = name;
        this.deviceId = deviceId;
    }

    public void setMinThreshold(double threshold) {
        this.minThreshold = threshold;
    }

    public void setValue(double value) {
        this.value = value;
    }

    public void setMaxThreshold(double threshold) {
        this.maxThreshold = threshold;
    }

    public void setDeviceId(String deviceId) {
        this.deviceId = deviceId;
    }

    @JsonProperty("deviceId")
    public String getDeviceId() {
        return deviceId;
    }

    @JsonProperty("sensor")
    public String getName() {
        return name;
    }

    @JsonProperty("value")
    public double getValue() {
        return value;
    }

    @JsonIgnore
    public double getMinThreshold() {
        return minThreshold;
    }

    @JsonIgnore
    public double getMaxThreshold() {
        return maxThreshold;
    }

}
