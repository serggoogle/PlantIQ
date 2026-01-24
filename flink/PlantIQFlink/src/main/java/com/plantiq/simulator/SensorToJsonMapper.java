package com.plantiq.simulator;

import com.plantiq.sensors.Sensor;
import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.databind.ObjectMapper;

public class SensorToJsonMapper implements MapFunction<Double, String> {
    private final Sensor sensor;
    private final ObjectMapper objectMapper = new ObjectMapper();
    public SensorToJsonMapper(Sensor sensor){
        this.sensor = sensor;
    }

    @Override
    public String map(Double value) throws Exception {
        sensor.setValue(value);
        return objectMapper.writeValueAsString(sensor);
    }
}
