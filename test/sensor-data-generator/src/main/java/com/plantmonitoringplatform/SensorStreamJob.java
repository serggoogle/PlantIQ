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

import org.apache.flink.api.common.eventtime.WatermarkStrategy;
import org.apache.flink.api.common.functions.FlatMapFunction;
import org.apache.flink.api.common.typeinfo.TypeInformation;
import org.apache.flink.api.connector.source.util.ratelimit.RateLimiterStrategy;
import org.apache.flink.connector.datagen.source.DataGeneratorSource;
import org.apache.flink.connector.datagen.source.GeneratorFunction;
import org.apache.flink.connector.prometheus.sink.PrometheusSink;
import org.apache.flink.connector.prometheus.sink.PrometheusTimeSeries;
import org.apache.flink.streaming.api.datastream.DataStreamSource;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.util.Collector;
import java.util.Random;

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
    private static double generateRandomNumber(double min, double max){
        return min + (new Random().nextDouble() * (max - min));
    }

	public static void main(String[] args) throws Exception {
		// Sets up the execution environment, which is the main entry point
		// to building Flink applications.
		final StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();;
        final long numberOfRecords = Long.MAX_VALUE;
        final double MIN_PLANT_TEMP = 0.0;
        final double MAX_PLANT_TEMP = 40.0;
        String prometheusRemoteWriteUrl = "http://host.docker.internal:9090/api/v1/write";

        // Datagen source
        GeneratorFunction<Long, PrometheusTimeSeries> temperatureGeneratorFunction = index ->
                PrometheusTimeSeries.builder()
                        .withMetricName("temperature_c")
                        .addLabel("plant-name", "flink-test-plant")
                        .addSample(generateRandomNumber(MIN_PLANT_TEMP, MAX_PLANT_TEMP), System.currentTimeMillis())
                        .build();

        DataGeneratorSource<PrometheusTimeSeries> temperatureSource = new DataGeneratorSource<>(
                        temperatureGeneratorFunction,
                        numberOfRecords,
                        RateLimiterStrategy.perSecond(1),
                        TypeInformation.of(PrometheusTimeSeries.class));

        // Stream of Datagen data
        DataStreamSource<PrometheusTimeSeries> temperatureDataStream = env.fromSource(temperatureSource, WatermarkStrategy.noWatermarks(),"Temperature Sensor");

        PrometheusSink prometheusSink = (PrometheusSink) PrometheusSink.builder()
                .setPrometheusRemoteWriteUrl(prometheusRemoteWriteUrl)
                .setHttpUserAgent(USER_AGENT)
                .build();


        temperatureDataStream.flatMap(new TimeSeriesToTimeSeries()).sinkTo(prometheusSink);
        temperatureDataStream.flatMap(new TimeSeriesToString()).print();

        // Execute program, beginning computation.
		env.execute( "Flink Java API Skeleton");
	}

    public static class TimeSeriesToTimeSeries implements FlatMapFunction<PrometheusTimeSeries, PrometheusTimeSeries> {
        @Override
        public void flatMap(PrometheusTimeSeries value, Collector<PrometheusTimeSeries> out) throws Exception {
            out.collect(value);
        }
    }
    
    public static class TimeSeriesToString implements FlatMapFunction<PrometheusTimeSeries, String> {
        @Override
        public void flatMap(PrometheusTimeSeries value, Collector<String> out) throws Exception {
            StringBuilder builder = new StringBuilder();
            for (PrometheusTimeSeries.Label label : value.getLabels()){
                builder.append("Label Name: " + label.getName() + ", Label value: " + label.getValue());
            }
            for (PrometheusTimeSeries.Sample sample : value.getSamples()){
                builder.append("\t↳Sample timeSample " + sample.getTimestamp() + ", Sample value: " + sample.getValue());
            }
            out.collect(builder.toString());
        }
    }

}
