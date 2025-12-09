#!/bin/bash
composeFiles=($(find infrastructure/ -name "*-compose.yml"))

for file in "${composeFiles[@]}"; do
	read -p "Tear down $file?[y/n]: " -n 1 -r
	[[ ! -z "$REPLY" ]] && echo
	if [[ $REPLY =~ ^[Yy]$ ]]
	then
		docker compose -f $file down
	fi
done

read -p "Tear down development environment?[y/n]: " -n 1 -r
[[ ! -z "$REPLY" ]] && echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    if [ "$(docker ps -a -q -f name=dev-container)" ]; then
    	docker stop dev-container > /dev/null  && docker rm dev-container > /dev/null
		echo "Removed dev container."
    fi
fi
