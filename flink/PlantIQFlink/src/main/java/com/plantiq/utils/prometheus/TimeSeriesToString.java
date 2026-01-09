package com.plantiq.utils.prometheus;

import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.connector.prometheus.sink.PrometheusTimeSeries;
import org.apache.flink.util.Collector;

public class TimeSeriesToString implements FlatMapFunction<PrometheusTimeSeries, String> {
    @Override
    public void flatMap(PrometheusTimeSeries value, Collector<String> out) throws Exception {
        StringBuilder builder = new StringBuilder();
        builder.append("Metric: " + value.getMetricName());
        for (PrometheusTimeSeries.Label label : value.getLabels()){
            builder.append("\n\tLabel Name: " + label.getName() + ", Label value: " + label.getValue());
        }
        for (PrometheusTimeSeries.Sample sample : value.getSamples()){
            builder.append("\n\tSample timeSample " + sample.getTimestamp() + ", Sample value: " + sample.getValue());
        }
        out.collect(builder.toString());
    }
}