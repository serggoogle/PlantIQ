#!/bin/bash
../create-flink-resources.sh -c simulator-engine-compose.yml && \
../submit-flink-job.sh -s -f ../PlantIQFlink/target/PlantIQFlink-1.0.jar -c simulator-engine-compose.yml