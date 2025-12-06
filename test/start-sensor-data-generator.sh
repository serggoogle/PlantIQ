#!/bin/bash

JAR_PATH="sensor-data-generator/target/sensor-data-generator-1.0.jar"
EXISTING_JAR=()

taskmanagers() {
    curl -sS http://localhost:8081/taskmanagers \
    | jq '.taskmanagers | length'
}

total_taskslots() {
    curl -sS http://localhost:8081/taskmanagers \
    | jq '[.taskmanagers[].slotsNumber] | add'
}

available_taskslots() {
    curl -sS http://localhost:8081/taskmanagers \
    | jq '[.taskmanagers[].freeSlots] | add'
}


scale_taskmanager(){
	newNumJobs="$1"
	runningJobs=$(($(total_taskslots) - $(available_taskslots)))
	totalNumJobs=$(($newNumJobs + $runningJobs))
	numTaskmanagers=$((($totalNumJobs + 1)/2)) # assuming 2 slots per taskmanager
	
	echo "Available taskslots: $(available_taskslots)"
	echo "Current number of taskmanagers: $(taskmanagers)"
	echo "Required number of taskmanagers: $numTaskmanagers"

	if [[ "$numTaskmanagers" -gt  $(taskmanagers) ]]; then
		echo "Scaling taskmanager to accommodate $totalNumJobs jobs (taskmanagers=$numTaskmanagers)"
		docker compose -f ../infrastructure/flink-compose.yml scale taskmanager=$numTaskmanagers
		sleep 3
	fi	
}

get_jar(){
	EXISTING_JAR=($(curl -sS -X GET --url http://localhost:8081/v1/jars | jq -r ".files[].id"))
}

delete_jar(){
	get_jar
	if [[ ! -z "$EXISTING_JAR"  ]]; then
 		echo "Removing existing jar files..."
		for file in "${EXISTING_JAR[@]}"; do
			echo " ↳$file"
			curl -sS -X DELETE --url http://localhost:8081/v1/jars/${file} > /dev/null
		done
	fi
}

upload_jar(){
	echo "Uploading latest jar..."
	res=$(curl -sS -X POST --url http://localhost:8081/v1/jars/upload --header 'content-type: multipart/form-data' -F "jarfile=@${JAR_PATH}" | jq -r ".status")

	if [[ ! ${res} = "success" ]]; then
		echo "Please check that Flink is running and the jar file exists at ${JAR_PATH}"
		echo "Upload failed."
		exit 1
	fi
}	

start_flink_jobs(){
	delete_jar
	upload_jar
	get_jar
	TASK_SLOTS=$(available_taskslots)
	echo "Creating $NUM_OF_JOBS sensor data generator(s)"
	echo "Using $EXISTING_JAR"
	echo "Number of running jobs: $runningJobs"
	
	scale_taskmanager $NUM_OF_JOBS
	
	for ((i=1; i < $NUM_OF_JOBS + 1; i++)); do
		local label="sensor-data-sim-$(openssl rand -hex 3)"
		jobId=$(curl -sS -X POST --url "http://localhost:8081/v1/jars/${EXISTING_JAR}/run?programArg=--label%2C%20$label" | jq -r '.jobid')
		echo Started $label with jobid $jobId
	done
}

while true; do
	read -p "Enter number of data simulators: " NUM_OF_JOBS
	if ! [[ "$NUM_OF_JOBS" =~ ^[0-9]+$ && "$NUM_OF_JOBS" -gt 0 ]]; then
		echo "Error: Please enter a valid number (1+ simulators)"
	else
		break;
	fi
done

start_flink_jobs

echo "Done"
