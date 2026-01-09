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

package com.plantiq;
import com.plantiq.sensors.HumiditySensor;
import com.plantiq.sensors.MoistureSensor;
import com.plantiq.sensors.Sensor;
import com.plantiq.sensors.TemperatureSensor;
import com.plantiq.utils.common.DoubleToStringMapper;
import com.plantiq.utils.common.TemperatureConversion;
import org.apache.flink.api.common.eventtime.WatermarkStrategy;
import org.apache.flink.api.common.serialization.SimpleStringSchema;
import org.apache.flink.api.common.typeinfo.TypeInformation;
import org.apache.flink.api.connector.source.util.ratelimit.RateLimiterStrategy;
import org.apache.flink.api.java.utils.ParameterTool;
import org.apache.flink.connector.datagen.source.DataGeneratorSource;
import org.apache.flink.connector.datagen.source.GeneratorFunction;
import org.apache.flink.connector.prometheus.sink.PrometheusSink;
import org.apache.flink.connector.prometheus.sink.PrometheusSinkConfiguration.*;
import org.apache.flink.streaming.api.datastream.DataStreamSource;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.connectors.rabbitmq.RMQSink;
import org.apache.flink.streaming.connectors.rabbitmq.common.RMQConnectionConfig;

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
    private static final String LABEL_NAME = "plant-name";
    private static final long NUMBER_OF_RECORDS = Long.MAX_VALUE;
    private static final int RECORDS_PER_SECOND = 20000;
    private static final String HOST = "host.docker.internal";
    private static final String RABBITMQ_VIRTUAL_HOST = "/";
    private static final String RABBITMQ_USERNAME = "guest";
    private static final String RABBITMQ_PASSWORD = "guest";
    private static final String PROMETHEUS_URL = "http://host.docker.internal:9090/api/v1/write";
    static final StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();
    private static ParameterTool params;

    private static double generateRandomNumber(double min, double max){
        return min + (new Random().nextDouble() * (max - min));
    }

    public static GeneratorFunction <Long, Double> sensorDataGenerator(Sensor sensor){
        return index -> generateRandomNumber(sensor.getMinThreshold(), sensor.getMaxThreshold());
    }

    public static DataGeneratorSource<Double> sensorDataSource(Sensor sensor){
        return new DataGeneratorSource<>(
                sensorDataGenerator(sensor),
                NUMBER_OF_RECORDS,
                RateLimiterStrategy.perSecond(RECORDS_PER_SECOND),
                TypeInformation.of(Double.class));
    }

    public static DataStreamSource<Double> sensorDataStream(DataGeneratorSource<Double> dataSource, String sourceName){
        return env.fromSource(dataSource, WatermarkStrategy.noWatermarks(), sourceName);
    }

	public static void main(String[] args) throws Exception {
        params = ParameterTool.fromArgs(args);

        TemperatureSensor temperatureSensor = new TemperatureSensor("temperature_c");
        MoistureSensor moistureSensor = new MoistureSensor("moisture");
        HumiditySensor humiditySensor = new HumiditySensor("humidity");

        DataGeneratorSource<Double> temperatureSource = sensorDataSource(temperatureSensor);
        DataGeneratorSource<Double> moistureSource = sensorDataSource(moistureSensor);
        DataGeneratorSource<Double> humiditySource = sensorDataSource(humiditySensor);

        // Stream of Datagen data
        DataStreamSource<Double> temperatureDataStream = sensorDataStream(temperatureSource, "Temperature Stream");
        DataStreamSource<Double> moistureDataStream = sensorDataStream(moistureSource, "Moisture Stream");
        DataStreamSource<Double> humidityDataStream = sensorDataStream(humiditySource, "Humidity Stream");

        // Stream Sinks
        final RMQConnectionConfig connectionConfig = new RMQConnectionConfig.Builder()
                .setHost(HOST)
                .setPort(5672)
                .setVirtualHost(RABBITMQ_VIRTUAL_HOST)
                .setUserName(RABBITMQ_USERNAME)
                .setPassword(RABBITMQ_PASSWORD)
                .build();
//        final PrometheusSink prometheusSink = (PrometheusSink) PrometheusSink.builder()
//                .setPrometheusRemoteWriteUrl(PROMETHEUS_URL)
//                .setHttpUserAgent(USER_AGENT)
//                .setRetryConfiguration(
//                        RetryConfiguration.builder()
//                                .setInitialRetryDelayMS(10L)
//                                .setMaxRetryDelayMS(5000L)
//                                .setMaxRetryCount(Integer.MAX_VALUE)
//                                .build()
//                )
//                .setErrorHandlingBehaviorConfiguration(
//                        SinkWriterErrorHandlingBehaviorConfiguration.builder()
//                                .onMaxRetryExceeded(OnErrorBehavior.DISCARD_AND_CONTINUE)
//                                .onPrometheusNonRetryableError(OnErrorBehavior.DISCARD_AND_CONTINUE)
//                                .build()
//                )
//                .build();

        // Sensor streams to Rabbitmq
        temperatureDataStream.map(new DoubleToStringMapper()).rebalance().addSink(new RMQSink<>(
           connectionConfig,
           "temperature-c",
            new SimpleStringSchema()
        )).name("Temperature-C-Sink");
        temperatureDataStream.map(new TemperatureConversion()).map(new DoubleToStringMapper()).rebalance().addSink(new RMQSink<>(
                connectionConfig,
                "temperature-f",
                new SimpleStringSchema()
        )).name("Temperature-F-Sink");
        moistureDataStream.map(new DoubleToStringMapper()).rebalance().addSink(new RMQSink<>(
                connectionConfig,
                "moisture",
                new SimpleStringSchema()
        )).name("Moisture-Sink");
        humidityDataStream.map(new DoubleToStringMapper()).rebalance().addSink(new RMQSink<>(
                connectionConfig,
                "humidity",
                new SimpleStringSchema()
        )).name("Humidity-Sink");

        // Execute program, beginning computation.
		env.execute(params.getRequired("label").strip());
	}

}
