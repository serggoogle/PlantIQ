package com.plantiq.utils.common;

import org.apache.flink.api.common.functions.RichMapFunction;

public class DoubleToStringMapper extends RichMapFunction<Double, String> {

    @Override
    public String map(Double value){
        return String.valueOf(value);
    }
}
