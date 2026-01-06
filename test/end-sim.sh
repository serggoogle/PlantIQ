#!/bin/bash

EXISTING_JOBS=()
HOST="http://localhost:8082"
COMPOSE="simulation-engine-compose.yml"

teardown_flink_simulation_engine(){
	echo "Tearing down Simulation Engine..."
	docker compose -f $COMPOSE down
}

scale_taskmanager_down(){
	echo "Scaling down TaskManagers..."
	docker compose -f $COMPOSE scale taskmanager-sim-engine=1
}

get_jobs(){
	EXISTING_JOBS=($(curl -sS -X GET --url $HOST/v1/jobs | jq -r ".jobs[].id"))
}

stop_jobs(){
	get_jobs
	echo "Stopping running jobs..."
	for job in "${EXISTING_JOBS[@]}"; do
		curl -sS -X PATCH --url $HOST/v1/jobs/${job} > /dev/null 
	done
}


stop_jobs
scale_taskmanager_down
teardown_flink_simulation_engine
echo "Done"
