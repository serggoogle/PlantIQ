#!/bin/bash

JAR_PATH="PlantIQFlink/target/PlantIQFlink-1.0.jar"
EXISTING_JAR=()
# Default flags
SIMULATION_ENGINE=false
HOST="http://localhost:8081"
COMPOSE="flink-compose.yml"
JOB_PARAMETERS=""
TASKMANAGER_NAME="taskmanager"
NUM_OF_JOBS="1"

taskmanagers() {
    curl -sS "$HOST/taskmanagers" \
    | jq '.taskmanagers | length'
}

total_taskslots() {
    curl -sS "$HOST/taskmanagers" \
    | jq '[.taskmanagers[].slotsNumber] | add'
}

available_taskslots() {
   curl -sS "$HOST/taskmanagers" \
    | jq '[.taskmanagers[].freeSlots] | add'
}


scale_taskmanager(){
	newNumJobs="$1"
	runningJobs=$(($(total_taskslots) - $(available_taskslots)))
	totalNumJobs=$(($newNumJobs + $runningJobs))
	numTaskmanagers=$((($totalNumJobs + 1)/2)) # assuming 2 slots per taskmanager
	echo "Number of running jobs: $runningJobs"
	echo "Available taskslots: $(available_taskslots)"
	echo "Current number of taskmanagers: $(taskmanagers)"
	echo "Required number of taskmanagers: $numTaskmanagers"

	if [[ "$numTaskmanagers" -gt  $(taskmanagers) ]]; then
		echo "Scaling taskmanager to accommodate $totalNumJobs jobs (taskmanagers=$numTaskmanagers)"
		docker compose -f $COMPOSE scale $TASKMANAGER_NAME=$numTaskmanagers
		sleep 3
	fi	
}

get_jar(){
	EXISTING_JAR=($(curl -sS -X GET --url "$HOST/v1/jars" | jq -r ".files[].id"))
}

build_jar(){
	echo "Building Flink jar..."
	docker exec dev-container bash -i -c "build-flink-jar" > /dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "Failed to build Flink jar."
		exit 1
	fi
	echo "> Flink jar built successfully."
}

delete_jar(){
	get_jar
	if [[ ! -z "$EXISTING_JAR"  ]]; then
 		echo "Removing existing jar files..."
		for file in "${EXISTING_JAR[@]}"; do
			echo " ↳$file"
			curl -sS -X DELETE --url "$HOST/v1/jars/${file}" > /dev/null
		done
	fi
}

upload_jar(){
	delete_jar
	build_jar
	echo "Uploading latest jar..."
	res=$(curl -sS -X POST --url "$HOST/v1/jars/upload" --header 'content-type: multipart/form-data' -F "jarfile=@${JAR_PATH}" | jq -r ".status")

	if [[ ! ${res} = "success" ]]; then
		echo ">Please check that Flink is running and the jar file exists at ${JAR_PATH}"
		echo "Upload failed."
		exit 1
	fi
	get_jar
}
start_flink_jobs(){
	upload_jar
	TASK_SLOTS=$(available_taskslots)
	echo "Creating $NUM_OF_JOBS jobs"
	echo "Using $EXISTING_JAR"
	scale_taskmanager $NUM_OF_JOBS
	for ((i=1; i < $NUM_OF_JOBS + 1; i++)); do
	    if [ "$SIMULATION_ENGINE" = true ] ; then
			local label="sim-$(openssl rand -hex 3)"
			jobId=$(curl -sS -X POST --url "$HOST/v1/jars/${EXISTING_JAR}/run?${JOB_PARAMETERS}${label}" | jq -r '.jobid')
		else
			jobId=$(curl -sS -X POST --url "$HOST/v1/jars/${EXISTING_JAR}/run" | jq -r '.jobid')
		fi
		if [ "$jobId" == "null" ]; then
			echo "Error submitting job. Please look at the Flink logs for more details"
			exit 1
		fi
		echo Started $label with jobid $jobId
	done
}


# Parse options: -s enable simulation engine, -H host, -c compose file, -h help
usage() {
		echo "Usage: $0 [-s] [-H host] [-c compose-file]"
		echo "  -s                Simulation engine mode"
		echo "  -H host           Flink REST host (default: $HOST)"
		echo "  -c compose-file   Docker compose file to use (default: $COMPOSE)"
}

while getopts ":f:sH:c:hn:" opt; do
	case ${opt} in
		s )
			SIMULATION_ENGINE=true
			COMPOSE="simulator/simulator-engine-compose.yml"
			JOB_PARAMETERS="entry-class=com.plantiq.SensorStreamJob&programArg=--label,"
			TASKMANAGER_NAME="simulator-taskmanager"
			;;
		H )
			HOST="$OPTARG"
			;;
		c )
			COMPOSE="$OPTARG"
			;;
		f )
			JAR_PATH="$OPTARG"
			;;
	  n)
	    NUM_OF_JOBS="$OPTARG"
	    ;;
		h )
			usage
			exit 0
			;;
		\? )
			echo "Invalid Option: -$OPTARG" 1>&2
			usage
			exit 1
			;;
		: )
			echo "Invalid option: -$OPTARG requires an argument" 1>&2
			usage
			exit 1
			;;
	esac
done
shift $((OPTIND -1))

start_flink_jobs