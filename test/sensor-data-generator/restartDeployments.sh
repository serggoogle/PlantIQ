#!/bin/bash
DEPLOYMENT="$1"
DEPLOYMENTS=()

NAMESPACE="default"
patch_deployment(){
  local STATE="$1"
  case "$STATE" in
    [CANCELLED]* );;
    [RUNNING]* );;
    * ) echo "Invalid PATCH Option"; exit 1
  esac
  curl -s -o /dev/null -w "%{http_code}\n" --location --request PATCH "http://127.0.0.1:8080/api/v1/namespaces/default/deployments/${DEPLOYMENT}" \
  --header 'Content-Type: application/json' \
  --data "{
    \"spec\": {
      \"state\": \"$STATE\"
    }
  }"
}

_get_deployment_names(){
  curl -s "http://127.0.0.1:9080/api/v1/namespaces/${NAMESPACE}/deployments" | grep '"name"' | sed -E 's/.*"name"[[:space:]]*:[[:space:]]*"([^"]+)".*/\1/'
}

isValidDeployment() {
  _get_deployment_names | grep -qx "$DEPLOYMENT"
}

_restart_deployment() {
  cancelReturnCode=$(patch_deployment "CANCELLED")
  runReturnCode=$(patch_deployment "RUNNING")
  if [[ $cancelReturnCode == "200" && $runReturnCode == "200" ]]; then
    echo "Successfully RESTARTED ${DEPLOYMENT}"
  fi
}

if [[ -z "$DEPLOYMENT" ]]; then
  echo "Restarting all deployments"
  DEPLOYMENTS=($(_get_deployment_names))
elif ! isValidDeployment; then
  echo "Invalid deployment: $DEPLOYMENT"
  echo "Valid deployments:"
  _get_deployment_names | tr ' ' '\n'
  exit 1
else
  DEPLOYMENTS=("$DEPLOYMENT")
fi

for deployment in "${DEPLOYMENTS[@]}"; do
  DEPLOYMENT=${deployment}
  _restart_deployment
done
