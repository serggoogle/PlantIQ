package com.plantiq.utils.prometheus;

import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.connector.prometheus.sink.PrometheusTimeSeries;
import org.apache.flink.util.Collector;

public class TimeSeriesToTimeSeries implements FlatMapFunction<Double, PrometheusTimeSeries> {
    @Override
    public void flatMap(Double value, Collector<PrometheusTimeSeries> out) throws Exception {
        // TODO: create a new Prometheus time series with the double
//        out.collect(value);
    }
}