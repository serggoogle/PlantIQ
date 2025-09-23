/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.plantmonitoringplatform;
import com.plantmonitoringplatform.sensors.HumiditySensor;
import com.plantmonitoringplatform.sensors.MoistureSensor;
import com.plantmonitoringplatform.sensors.Sensor;
import com.plantmonitoringplatform.sensors.TemperatureSensor;
import org.apache.flink.api.common.eventtime.WatermarkStrategy;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.api.common.typeinfo.TypeInformation;
import org.apache.flink.api.connector.source.util.ratelimit.RateLimiterStrategy;
import org.apache.flink.connector.datagen.source.DataGeneratorSource;
import org.apache.flink.connector.datagen.source.GeneratorFunction;
import org.apache.flink.connector.prometheus.sink.PrometheusSink;
import org.apache.flink.connector.prometheus.sink.PrometheusSinkConfiguration.*;
import org.apache.flink.connector.prometheus.sink.PrometheusTimeSeries;
import org.apache.flink.connector.prometheus.sink.PrometheusTimeSeriesLabelsAndMetricNameKeySelector;
import org.apache.flink.streaming.api.datastream.DataStreamSource;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.util.Collector;
import org.apache.flink.util.ParameterTool;
import java.util.Random;
import java.util.logging.Logger;

import static org.apache.flink.shaded.curator5.com.google.common.net.HttpHeaders.USER_AGENT;

/**
 * Skeleton for a Flink DataStream Job.
 *
 * <p>For a tutorial how to write a Flink application, check the
 * tutorials and examples on the <a href="https://flink.apache.org">Flink Website</a>.
 *
 * <p>To package your application into a JAR file for execution, run
 * 'mvn clean package' on the command line.
 *
 * <p>If you change the name of the main class (with the public static void main(String[] args))
 * method, change the respective entry in the POM.xml file (simply search for 'mainClass').
 */
public class SensorStreamJob {
    private static final String LABEL_NAME = "plant-name";
    private static final long NUMBER_OF_RECORDS = Long.MAX_VALUE;
    private static final int RECORDS_PER_SECOND = 1;
    private static final String PROMETHEUS_URL = "http://host.docker.internal:9090/api/v1/write";
    static final StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();

    private static double generateRandomNumber(double min, double max){
        return min + (new Random().nextDouble() * (max - min));
    }

    private static double celsiusToFahrenheit(double temperature){
        return (temperature * 9/5) + 32;
    }

    public static GeneratorFunction <Long, PrometheusTimeSeries> sensorDataGenerator(Sensor sensor, String labelValue){
        return index ->
                PrometheusTimeSeries.builder()
                        .withMetricName(sensor.getName())
                        .addLabel(LABEL_NAME, labelValue)
                        .addSample(generateRandomNumber(sensor.getMinThreshold(), sensor.getMaxThreshold()), System.currentTimeMillis())
                        .build();
    }

    public static DataGeneratorSource<PrometheusTimeSeries> sensorDataSource(Sensor sensor, String labelValue){
        return new DataGeneratorSource<>(
                sensorDataGenerator(sensor, labelValue),
                NUMBER_OF_RECORDS,
                RateLimiterStrategy.perSecond(RECORDS_PER_SECOND),
                TypeInformation.of(PrometheusTimeSeries.class));
    }

    public static DataStreamSource<PrometheusTimeSeries> sensorDataStream(DataGeneratorSource<PrometheusTimeSeries> dataSource, String sourceName){
        return env.fromSource(dataSource, WatermarkStrategy.noWatermarks(), sourceName);
    }

	public static void main(String[] args) throws Exception {
        ParameterTool params = ParameterTool.fromArgs(args);
        final String labelValue = params.getRequired("label");

        TemperatureSensor temperatureSensor = new TemperatureSensor("temperature_c");
        MoistureSensor moistureSensor = new MoistureSensor("moisture");
        HumiditySensor humiditySensor = new HumiditySensor("humidity");
        DataGeneratorSource<PrometheusTimeSeries> temperatureSource = sensorDataSource(temperatureSensor, labelValue);
        DataGeneratorSource<PrometheusTimeSeries> moistureSource = sensorDataSource(moistureSensor, labelValue);
        DataGeneratorSource<PrometheusTimeSeries> humiditySource = sensorDataSource(humiditySensor, labelValue);

        // Stream of Datagen data
        DataStreamSource<PrometheusTimeSeries> temperatureDataStream = sensorDataStream(temperatureSource, "Temperature-C Stream");
        DataStreamSource<PrometheusTimeSeries> moistureDataStream = sensorDataStream(moistureSource, "Moisture Stream");
        DataStreamSource<PrometheusTimeSeries> humidityDataStream = sensorDataStream(humiditySource, "Humidity Stream");

        PrometheusSink prometheusSink = (PrometheusSink) PrometheusSink.builder()
                .setPrometheusRemoteWriteUrl(PROMETHEUS_URL)
                .setHttpUserAgent(USER_AGENT)
                .setRetryConfiguration(
                        RetryConfiguration.builder()
                                .setInitialRetryDelayMS(10L)
                                .setMaxRetryDelayMS(5000L)
                                .setMaxRetryCount(Integer.MAX_VALUE)
                                .build()
                )
                .setErrorHandlingBehaviorConfiguration(
                        SinkWriterErrorHandlingBehaviorConfiguration.builder()
                                .onMaxRetryExceeded(OnErrorBehavior.DISCARD_AND_CONTINUE)
                                .onPrometheusNonRetryableError(OnErrorBehavior.DISCARD_AND_CONTINUE)
                                .build()
                )
                .build();


        temperatureDataStream
                .flatMap(new TimeSeriesToTimeSeries())
                .keyBy(new PrometheusTimeSeriesLabelsAndMetricNameKeySelector())
                .sinkTo(prometheusSink).name("Temperature-C-Sink");
        temperatureDataStream
                .map(new TemperatureConversion())
                .keyBy(new PrometheusTimeSeriesLabelsAndMetricNameKeySelector())
                .sinkTo(prometheusSink)
                .name("Temperature-F-Sink");
        moistureDataStream
                .flatMap(new TimeSeriesToTimeSeries())
                .keyBy(new PrometheusTimeSeriesLabelsAndMetricNameKeySelector())
                .sinkTo(prometheusSink)
                .name("Moisture-Sink");
        humidityDataStream
                .flatMap(new TimeSeriesToTimeSeries())
                .keyBy(new PrometheusTimeSeriesLabelsAndMetricNameKeySelector())
                .sinkTo(prometheusSink)
                .name("Humidity-Sink");

//        temperatureDataStream.flatMap(new TimeSeriesToString()).print();
//        temperatureDataStream.map(new TemperatureConversion()).flatMap(new TimeSeriesToString()).print();
//        moistureDataStream.flatMap(new TimeSeriesToString()).print();
//        humidityDataStream.flatMap(new TimeSeriesToString()).print();

        // Execute program, beginning computation.
		env.execute( "Sensor Stream Job");
	}

    public static class TimeSeriesToTimeSeries implements FlatMapFunction<PrometheusTimeSeries, PrometheusTimeSeries> {
        @Override
        public void flatMap(PrometheusTimeSeries value, Collector<PrometheusTimeSeries> out) throws Exception {
            out.collect(value);
        }
    }

    public static class TemperatureConversion implements MapFunction<PrometheusTimeSeries, PrometheusTimeSeries> {
        @Override
        public PrometheusTimeSeries map(PrometheusTimeSeries series){
            double sample = -1;
            long timestamp = 0L;
            String labelValue = series.getLabels()[0].getValue();
            for (PrometheusTimeSeries.Sample seriesSample : series.getSamples()){
                sample = celsiusToFahrenheit(seriesSample.getValue());
                timestamp = seriesSample.getTimestamp();
            }
            return PrometheusTimeSeries.builder()
                    .withMetricName("temperature_f")
                    .addLabel(LABEL_NAME, labelValue)
                    .addSample(sample,timestamp)
                    .build();
        }
    }

    public static class TimeSeriesToString implements FlatMapFunction<PrometheusTimeSeries, String> {
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

}
