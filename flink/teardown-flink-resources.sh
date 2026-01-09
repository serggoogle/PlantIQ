#!/bin/bash

EXISTING_JOBS=()
HOST="http://localhost:8081"
SIMULATION_ENGINE=false
COMPOSE="flink-compose.yml"

teardown_flink(){
	echo "Tearing down Flink Resources..."
	docker compose -f $COMPOSE down
}

get_jobs(){
	if [ "$SIMULATION_ENGINE" = true ] ; then
		echo "Getting running simulation jobs..."
		EXISTING_JOBS=($(curl -sS -X GET --url $HOST/jobs/overview | jq -r '.jobs[] | select(.name | test("^sim")) | .jid'))
	else
		EXISTING_JOBS=($(curl -sS -X GET --url $HOST/v1/jobs | jq -r ".jobs[].id"))
	fi
}

stop_jobs(){
	get_jobs
	echo "Stopping running jobs..."
	for job in "${EXISTING_JOBS[@]}"; do
		curl -sS -X PATCH --url $HOST/v1/jobs/${job} > /dev/null 
	done
}

usage() {
		echo "Usage: $0 [-s] [-c compose-file]"
		echo "  -s                Simulation engine mode"
		echo "  -c compose-file   Docker compose file to use (default: $COMPOSE)"
}

while getopts "c:sh" opt; do
	case ${opt} in
		s) 
			SIMULATION_ENGINE=true
			;;
		c)
			echo Setting compose file to $OPTARG
			COMPOSE="$OPTARG"
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


stop_jobs
teardown_flink
