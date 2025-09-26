NAMESPACE="default"

_get_deployment_names(){
  curl -s "http://127.0.0.1:9080/api/v1/namespaces/${NAMESPACE}/deployments" | grep '"name"' | sed -E 's/.*"name"[[:space:]]*:[[:space:]]*"([^"]+)".*/\1/'
}

_deployment_autocomplete() {
  local cur_word prev_word
  cur_word="${COMP_WORDS[COMP_CWORD]}"
  prev_word="${COMP_WORDS[COMP_CWORD-1]}"

  # Fetch deployments dynamically — for example from kubectl
  local deployments=$(_get_deployment_names)
  # Return possible completions
  COMPREPLY=( $(compgen -W "${deployments}" -- "$cur_word") )
}

# Register the completion function for the script
complete -F _deployment_autocomplete restartDeployments.sh
complete -F _deployment_autocomplete stopDeployments.sh