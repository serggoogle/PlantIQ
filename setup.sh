#!/bin/bash

setup_dev_environment() {
    echo "Building development environment..."
    docker build -f dev-env.Dockerfile -t dev-image . && \
        docker run --name dev-container \
		-v $PWD/:/opt/plant-monitoring-platform/ \
        -v ~/.m2/repository:/root/.m2/repository \
		-d dev-image
    if [ $? -ne 0 ]; then
        echo "Failed to build or run the development container."
        exit 1
    fi
}

setup_infrastructure() {
    echo "Setting up metrics infrastructure..."
    docker-compose -f infrastructure/metrics-compose.yml -f infrastructure/flink-compose.yml up -d
    if [ $? -ne 0 ]; then
        echo "Failed to build infrastructure services."
        exit 1
    fi
}

if [[ -z $(docker ps --filter name=dev-container --format={{.ID}}) ]]; then
	setup_dev_environment && echo "Development container successfuly created" 
fi

setup_infrastructure && echo "All services are up and running."
