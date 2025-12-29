#!/bin/bash
GF_DEFAULT_USER_PASS="admin:admin"
create_prometheus_ds(){
    NUM_OF_PROM_DS=$(curl -u $GF_DEFAULT_USER_PASS -sS 'http://localhost:3000/api/datasources' | jq '.[].name ' | grep prometheus | wc -l)
    if [ $NUM_OF_PROM_DS -eq 0 ]; then
        echo "Creating Prometheus Datasource..."
        POST_MESSAGE=$(curl -u $GF_DEFAULT_USER_PASS -sS -X POST http://localhost:3000/api/datasources \
        -H 'content-type: application/json' \
        -d @infrastructure/grafana/Prometheus-Datasource-Template.json | jq -r .message)
        echo "> $POST_MESSAGE"
        sleep 3
    fi
}

import_metrics_dashboard(){
    echo "Importing Dashboard..."
    DASHBOARD_IMPORTED=$(curl -u $GF_DEFAULT_USER_PASS -sS -X POST http://localhost:3000/api/dashboards/import \
    -H 'accept: application/json' -H 'Content-Type: application/json' \
    -d @infrastructure/grafana/Plant-Metrics-Template.json | jq '.imported')
    if [ "$DASHBOARD_IMPORTED" = "true" ]; then
        echo "> Dashboard successfully imported"
    fi
}

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

setup_grafana(){
    echo "Setting up Grafana..."
    sleep 2
    create_prometheus_ds
    import_metrics_dashboard
}

setup_infrastructure() {
    echo "Setting up metrics infrastructure..."
    docker-compose -f infrastructure/metrics-compose.yml -f infrastructure/flink-compose.yml up -d --wait
    if [ $? -ne 0 ]; then
        echo "Failed to build infrastructure services."
        exit 1
    fi
    echo "All Services are up and running"
}

if [[ -z $(docker ps --filter name=dev-container --format={{.ID}}) ]]; then
	setup_dev_environment && echo "Development container successfuly created" 
fi

setup_infrastructure
setup_grafana
echo "Done"