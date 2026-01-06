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

import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.api.common.serialization.SimpleStringSchema;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.datastream.DataStreamSource;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.connectors.rabbitmq.RMQSource;
import org.apache.flink.streaming.connectors.rabbitmq.common.RMQConnectionConfig;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

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
public class RabbitMQKafkaStream {
    // TODO: Investigate using ENV variables instead since
    private static final String HOST = "host.docker.internal";
    private static final String RABBITMQ_VIRTUAL_HOST = "/";
    private static final String RABBITMQ_USERNAME = "guest";
    private static final String RABBITMQ_PASSWORD = "guest";
    private static StreamExecutionEnvironment env;

    private static RMQSource<String> RMQSourceStream(RMQConnectionConfig config, String queueName){
        return new RMQSource<>(
                        config,            // config for the RabbitMQ connection
                        queueName,                 // name of the RabbitMQ queue to consume
                        true,                        // use correlation ids; can be false if only at-least-once is required
                        new SimpleStringSchema());  // deserialization schema to turn messages into Java objects
    }

    private static class printDebugger implements MapFunction<String, String> {
        private String queueName = null;
        public printDebugger(String queueName){
            this.queueName = queueName;
        }
        @Override
        public String map(String s){
            return queueName +": " + s;
        }
    }

    private static class StringToDoubleMapper implements MapFunction<String, Double> {

        @Override
        public Double map(String s) throws Exception {
            return Double.parseDouble(s);
        }
    }

	public static void main(String[] args) throws Exception {
        HashMap<String, ArrayList<DataStream<String>>> queueStreamMap = new HashMap<>();
        List<String> queues = new ArrayList<>();
        ArrayList<DataStream<String>> RMQStreams = new ArrayList<>();
        queues.add("temperature-f");
        queues.add("temperature-c");
        queues.add("humidity");
        queues.add("moisture");
        env = StreamExecutionEnvironment.getExecutionEnvironment();
        final RMQConnectionConfig rmqConfig = new RMQConnectionConfig.Builder()
                .setHost(HOST)
                .setPort(5672)
                .setVirtualHost(RABBITMQ_VIRTUAL_HOST)
                .setUserName(RABBITMQ_USERNAME)
                .setPassword(RABBITMQ_PASSWORD)
                .build();

        // TODO: Define RabbitMQ Sources
        for (String queue : queues){
            RMQStreams.add(env.addSource(RMQSourceStream(rmqConfig, queue)));
        }

        // Temp print - implement kafka sink later.
        for (HashMap<,>: queueStreamMap){
            source.map(new printDebugger()).print();
        }

        // TODO: Define Kafka Sink

		env.execute("Flink Java API Skeleton");
	}
}
