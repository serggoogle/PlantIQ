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
import org.apache.flink.connector.base.DeliveryGuarantee;
import org.apache.flink.connector.kafka.sink.KafkaRecordSerializationSchema;
import org.apache.flink.connector.kafka.sink.KafkaSink;
import org.apache.flink.formats.json.JsonSerializationSchema;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.flink.shaded.jackson2.com.fasterxml.jackson.databind.SerializationFeature;
import org.apache.flink.streaming.api.datastream.DataStreamSource;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.connectors.rabbitmq.RMQSource;
import org.apache.flink.streaming.connectors.rabbitmq.common.RMQConnectionConfig;

import java.util.*;

public class RabbitMQKafkaStream {
    // TODO: Investigate using ENV variables instead since
    private static final String HOST = "host.docker.internal";
    private static final String RABBITMQ_VIRTUAL_HOST = "/";
    private static final String RABBITMQ_USERNAME = "guest";
    private static final String RABBITMQ_PASSWORD = "guest";
    private static final short RABBITMQ_PORT = 5672;
    private static final String KAFKA_BOOTSTRAP_SERVER = "broker:9092";
    private static final String KAFKA_TOPIC = "test-topic";
    private static StreamExecutionEnvironment env;

    private static RMQSource<String> RMQSourceStream(RMQConnectionConfig config, String queueName){
        return new RMQSource<>(
                        config,            // config for the RabbitMQ connection
                        queueName,                 // name of the RabbitMQ queue to consume
                        true,                        // use correlation ids; can be false if only at-least-once is required
                        new SimpleStringSchema());  // deserialization schema to turn messages into Java objects
    }

    private static class printDebugger implements MapFunction<String, String> {
        @Override
        public String map(String s) throws Exception {
            return s;
        }
    }

	public static void main(String[] args) throws Exception {
        HashMap<String, DataStreamSource<String>> queueStreamMap = new HashMap<>();
        List<String> queues = new ArrayList<>();
        Collections.addAll(
                queues,
                "temperature-f",
                "temperature-c",
                "humidity",
                "moisture"
        );
        env = StreamExecutionEnvironment.getExecutionEnvironment();
        final RMQConnectionConfig rmqConfig = new RMQConnectionConfig.Builder()
                .setHost(HOST)
                .setPort(RABBITMQ_PORT)
                .setVirtualHost(RABBITMQ_VIRTUAL_HOST)
                .setUserName(RABBITMQ_USERNAME)
                .setPassword(RABBITMQ_PASSWORD)
                .build();

        for (String queue : queues){
            queueStreamMap.put(queue,
                    env.addSource(RMQSourceStream(rmqConfig, queue), "RMQ-"+queue));
        }

        // Temp print - implement kafka sink later.
        queueStreamMap.forEach((queue, source) ->
                source.map(new printDebugger()).rebalance().print().name(queue)
        );

        queueStreamMap.forEach((queue, source) ->
                source.sinkTo(KafkaSink.<String>builder()
                        .setBootstrapServers(KAFKA_BOOTSTRAP_SERVER)
                        .setRecordSerializer(KafkaRecordSerializationSchema.builder()
                                .setTopic(queue)
                                .setValueSerializationSchema(new JsonSerializationSchema<String>(
                                        () -> new ObjectMapper()
                                            .enable(SerializationFeature.ORDER_MAP_ENTRIES_BY_KEYS))
                                )
                                .build()
                        )
                        .setDeliveryGuarantee(DeliveryGuarantee.AT_LEAST_ONCE)
                        .build())
                .name("Kafka-"+queue)
        );

		env.execute("RabbitMQKafkaStream");
	}
}
