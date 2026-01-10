#!/bin/bash
GF_DEFAULT_USER_PASS="admin:admin"
create_prometheus_ds(){
    NUM_OF_PROM_DS=$(curl -u $GF_DEFAULT_USER_PASS -sS 'http://localhost:3000/api/datasources' | jq '.[].name ' | grep prometheus | wc -l)
    if [ $NUM_OF_PROM_DS -eq 0 ]; then
        echo "Creating Prometheus Datasource..."
        POST_MESSAGE=$(curl -u $GF_DEFAULT_USER_PASS -sS -X POST http://localhost:3000/api/datasources \
        -H 'content-type: application/json' \
        -d @infrastructure/grafana/datasources/Prometheus-Datasource.json | jq -r .message)
        echo "> $POST_MESSAGE"
        sleep 3
    fi
}

import_dashboards(){
    echo "Importing Dashboards..."
    dashboards=($(ls infrastructure/grafana/dashboards))
    for dashboard in "${dashboards[@]}"; do
        DASHBOARD_IMPORTED=$(curl -u $GF_DEFAULT_USER_PASS -sS -X POST http://localhost:3000/api/dashboards/import \
        -H 'accept: application/json' -H 'Content-Type: application/json' \
        -d @infrastructure/grafana/dashboards/$dashboard | jq '.imported')
        if [ "$DASHBOARD_IMPORTED" = "true" ]; then
            echo "> $dashboard dashboard successfully imported"
        fi
        sleep 1
    done
}

setup_dev_environment() {
    echo "Building development environment..."
    docker build -f dev-env.Dockerfile -t dev-image . && \
        docker run --name dev-container \
		-v $PWD/:/opt/PlantIQ/ \
        -v ~/.m2/repository:/root/.m2/repository \
		-d dev-image
    if [ $? -ne 0 ]; then
        echo "Failed to build or run the development container."
        exit 1
    fi
}

setup_grafana(){
    echo "Setting up Grafana..."
    sleep 3
    create_prometheus_ds
    import_dashboards
}

setup_flink(){
    (./flink/create-flink-resources.sh -c flink/flink-compose.yml && \
    ./flink/submit-flink-job.sh -f flink/PlantIQFlink/target/PlantIQFlink-1.0.jar) || exit 1
}

setup_networks() {
    networks=(metrics simulator flink messaging)
    for network in "${networks[@]}"; do
        if [[ -z $(docker network ls --filter name=${network} --format={{.ID}}) ]]; then
            echo "> Creating ${network}..."
            docker network create ${network} > /dev/null
        fi
    done
}

setup_infrastructure() {
    echo "Setting up metrics infrastructure..."
    composeFiles=($(ls infrastructure/*.yml))
    for file in "${composeFiles[@]}"; do
        docker-compose -f "$file" up -d --wait
        if [ $? -ne 0 ]; then
            echo "Failed to build $file infrastructure services."
            exit 1
        fi
    done
    echo "All Services are up and running"
}

if [[ -z $(docker ps --filter name=dev-container --format={{.ID}}) ]]; then
	setup_dev_environment && echo "Development container successfuly created" 
fi

setup_networks
setup_infrastructure
setup_flink
setup_grafana
echo "Done"