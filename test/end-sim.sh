#!/bin/bash

EXISTING_JOBS=()

scale_taskmanager_down(){
	echo "Scaling down TaskManagers..."
	docker compose -f ../infrastructure/flink-compose.yml scale taskmanager=1
}

get_jobs(){
	EXISTING_JOBS=($(curl -sS -X GET --url http://localhost:8081/v1/jobs | jq -r ".jobs[].id"))
}

stop_jobs(){
	get_jobs
	echo "Stopping running jobs..."
	for job in "${EXISTING_JOBS[@]}"; do
		curl -sS -X PATCH --url http://localhost:8081/v1/jobs/${job} > /dev/null 
	done
}

stop_jobs
scale_taskmanager_down
echo "Done"
