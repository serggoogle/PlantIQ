package com.plantiq.utils.common;

import org.apache.flink.api.common.functions.MapFunction;

public class TemperatureConversion implements MapFunction<Double, Double> {
    private static double celsiusToFahrenheit(double temperature){
        return (temperature * 9/5) + 32;
    }

    @Override
    public Double map(Double value){
        return celsiusToFahrenheit(value);
    }
}