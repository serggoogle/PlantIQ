#!/bin/bash
COMPOSE="flink-compose.yml"

create_flink_resources(){
	echo "Creating flink environment..."
	docker compose -f $COMPOSE up -d  && sleep 3 || exit 1
}

# Parse options: -s enable simulation engine, -H host, -c compose file, -h help
usage() {
		echo "Usage: $0 [-c compose-file]"
		echo "  -c compose-file   Docker compose file to use (default: $COMPOSE)"
}

while getopts ":c:h" opt; do
	case ${opt} in
		c )
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
create_flink_resources