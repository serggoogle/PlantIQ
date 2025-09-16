#!/bin/bash
# Simple script to build, upload and restart the flink job using the REST API of Ververica Platform
_upload_jar(){
  curl -X 'POST' \
    'http://localhost:8080/artifacts/v1/namespaces/default/artifacts:upload' \
    -H 'accept: application/json' \
    -H 'Content-Type: multipart/form-data' \
    -F 'file=@target/data-gen-1.0-SNAPSHOT.jar'
}
_deployment_patch(){
  # Update state of the deployment (RUNNING/CANCELLED)
  local deployment="$1"
  local state="$2"
  echo "Sending deployment PATCH to "
  curl -X 'PATCH' \
    "http://127.0.0.1:9080/api/v1/namespaces/default/deployments/$deployment" \
    -H 'accept: */*' \
    -H 'Content-Type: application/json' \
    -d "{
    \"spec\" : {
        \"state\" : \"$state\"
    },
    \"status\": {
      \"state\": \"$state\"
      }
  }" >> test.log
}

# _deployment_patch "datagen-to-prometheus-su1po" "CANCELLED" &&
  mvn clean package &&
  _upload_jar &&
  # _deployment_patch "datagen-to-prometheus-su1po" "RUNNING" &&
  echo "Done"

