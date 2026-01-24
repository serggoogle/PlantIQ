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
import com.plantiq.simulator.SensorToJsonMapper;
import org.apache.flink.api.common.eventtime.WatermarkStrategy;
import org.apache.flink.api.common.serialization.SimpleStringSchema;
import org.apache.flink.api.common.typeinfo.TypeInformation;
import org.apache.flink.api.connector.source.util.ratelimit.RateLimiterStrategy;
import org.apache.flink.api.java.utils.ParameterTool;
import org.apache.flink.connector.datagen.source.DataGeneratorSource;
import org.apache.flink.connector.datagen.source.GeneratorFunction;
import org.apache.flink.streaming.api.datastream.DataStreamSource;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.connectors.rabbitmq.RMQSink;
import org.apache.flink.streaming.connectors.rabbitmq.common.RMQConnectionConfig;

import java.util.Random;

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
    private static final long NUMBER_OF_RECORDS = Long.MAX_VALUE;
    private static final int RECORDS_PER_SECOND = 1;
    private static final String HOST = "host.docker.internal";
    private static final String RABBITMQ_VIRTUAL_HOST = "/";
    private static final short RABBITMQ_PORT = 5672;
    private static final String RABBITMQ_USERNAME = "guest";
    private static final String RABBITMQ_PASSWORD = "guest";
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
        final String DEVICE_ID = params.getRequired("label");
        TemperatureSensor temperatureC_Sensor = new TemperatureSensor("temperature_c", DEVICE_ID, false);
        TemperatureSensor temperatureF_Sensor = new TemperatureSensor("temperature_f", DEVICE_ID, true);
        MoistureSensor moistureSensor = new MoistureSensor("moisture", DEVICE_ID);
        HumiditySensor humiditySensor = new HumiditySensor("humidity", DEVICE_ID);

        DataGeneratorSource<Double> temperatureC_Source = sensorDataSource(temperatureC_Sensor);
        DataGeneratorSource<Double> temperatureF_Source = sensorDataSource(temperatureF_Sensor);
        DataGeneratorSource<Double> moistureSource = sensorDataSource(moistureSensor);
        DataGeneratorSource<Double> humiditySource = sensorDataSource(humiditySensor);

        // Stream of Datagen data
        DataStreamSource<Double> temperatureC_DataStream = sensorDataStream(temperatureC_Source, "Temperature-C Stream");
        DataStreamSource<Double> temperatureF_DataStream = sensorDataStream(temperatureF_Source, "Temperature-F Stream");
        DataStreamSource<Double> moistureDataStream = sensorDataStream(moistureSource, "Moisture Stream");
        DataStreamSource<Double> humidityDataStream = sensorDataStream(humiditySource, "Humidity Stream");

        // Stream Sinks
        final RMQConnectionConfig connectionConfig = new RMQConnectionConfig.Builder()
                .setHost(HOST)
                .setPort(RABBITMQ_PORT)
                .setVirtualHost(RABBITMQ_VIRTUAL_HOST)
                .setUserName(RABBITMQ_USERNAME)
                .setPassword(RABBITMQ_PASSWORD)
                .build();

        // Sensor streams to Rabbitmq
        temperatureC_DataStream.map(new SensorToJsonMapper(temperatureC_Sensor)).rebalance().addSink(new RMQSink<>(
               connectionConfig,
               "temperature-c",
                new SimpleStringSchema()
        )).name("Temperature-C-Sink");
        temperatureF_DataStream.map(new SensorToJsonMapper(temperatureF_Sensor)).rebalance().addSink(new RMQSink<>(
                connectionConfig,
                "temperature-f",
                new SimpleStringSchema()
        )).name("Temperature-F-Sink");
        moistureDataStream.map(new SensorToJsonMapper(moistureSensor)).rebalance().addSink(new RMQSink<>(
                connectionConfig,
                "moisture",
                new SimpleStringSchema()        )).name("Moisture-Sink");
        humidityDataStream.map(new SensorToJsonMapper(humiditySensor)).rebalance().addSink(new RMQSink<>(
                connectionConfig,
                "humidity",
                new SimpleStringSchema()
        )).name("Humidity-Sink");

        // Execute program, beginning computation.
		env.execute(params.getRequired("label").strip());
	}

}
