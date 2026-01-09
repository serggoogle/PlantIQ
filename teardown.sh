#!/bin/bash
composeFiles=($(find infrastructure/ -name "*-compose.yml"))
volumes=($(docker volume ls | grep infrastructure_ | awk '{print $2}'))
TEARDOWN_ALL_SERVICES="$1"

delete_infrastructure_volumes(){
	for volume in "${volumes[@]}"; do
		docker volume rm $volume > /dev/null && echo "$volume deleted"
	done
}

teardown_dev_environment(){
    if [ "$(docker ps -a -q -f name=dev-container)" ]; then
    	docker stop dev-container > /dev/null  && docker rm dev-container > /dev/null
		echo "Removed dev container."
    fi
}

teardown_flink_environment(){
	./flink/teardown-flink-resources.sh -c flink/flink-compose.yml
	docker exec dev-container bash -i -c "clean-flink-target" > /dev/null 2>&1
	echo "Flink environment torn down."
}

teardown_all_services(){
	teardown_flink_environment
	for file in "${composeFiles[@]}"; do
		docker compose -f $file down
	done
	delete_infrastructure_volumes
	teardown_dev_environment
}

if [[ "$TEARDOWN_ALL_SERVICES" == "-a" ]]; then
	teardown_all_services
	exit 0
fi

teardown_flink_environment

for file in "${composeFiles[@]}"; do
	read -p "Teardown $file?[y/n]: " -n 1 -r
	[[ ! -z "$REPLY" ]] && echo
	if [[ $REPLY =~ ^[Yy]$ ]]; then
		docker compose -f $file down
	fi
done

read -p "Remove infrastructure volumes?[y/n]: " -n 1 -r 
[[ ! -z "$REPLY" ]] && echo 
if [[ $REPLY =~ ^[Yy]$ ]]; then
	delete_infrastructure_volumes
fi

read -p "Teardown development environment?[y/n]: " -n 1 -r
[[ ! -z "$REPLY" ]] && echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
	teardown_dev_environment
fi
