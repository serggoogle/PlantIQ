package com.plantiq.sensors;

import com.plantiq.Plant;
import java.time.Instant;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.annotation.JsonCreator;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.annotation.JsonIgnore;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.annotation.JsonProperty;

import java.io.Serializable;
import java.util.Objects;

public class Sensor extends Plant implements Serializable {

    private String sensorName = "";
    private double minThreshold = 0.0;
    private double maxThreshold = 0.0;
    private double value = 0.0;
    private Long timestamp = 0L;
    private static final Sensor sensor = null;


    @JsonCreator(mode = JsonCreator.Mode.PROPERTIES)
    public Sensor(String sensorName) {
        super("deviceId","plant_name", "plant_type", "species");
        this.sensorName = sensorName;
    }

    public Sensor(String deviceId, String plantName, String sensorName) {
        super(deviceId, plantName, "unknown", "unknown");
        this.sensorName = sensorName;
        this.setTimestamp();
    }

    public Sensor(
            String deviceId,
            String name,
            String type,
            String species,
            String sensorName
    )
    {
        super(deviceId, name, type, species);
        this.sensorName = sensorName;
    }

    public static Sensor TEST_SENSOR() {
        String testDeviceId = "ESP-32-Host";
        String testPlantName = "tomato";
        String testSensorName = "temperature_sensor";
        return Objects.requireNonNullElseGet(sensor, () -> new Sensor(testDeviceId, testPlantName, testSensorName));
    }

    public void setMinThreshold(double threshold) {
        this.minThreshold = threshold;
    }

    public void setValue(double value) {
        this.value = value;
    }

    public void setTimestamp()
    {
        this.timestamp = Instant.now().getEpochSecond();
    }

    public void setTimestamp(Long timestamp)
    {
        this.timestamp = timestamp;
    }

    public void setMaxThreshold(double threshold) {
        this.maxThreshold = threshold;
    }

    @JsonProperty("value")
    public double getValue() {
        return value;
    }

    @JsonProperty("sensor_name")
    public String getSensorName(){
        return sensorName;
    }

    @JsonProperty("timestamp")
    public Long getTimestamp() { return timestamp; }

    @JsonIgnore
    public double getMinThreshold() {
        return minThreshold;
    }

    @JsonIgnore
    public double getMaxThreshold() {
        return maxThreshold;
    }

}
