# /etc/bash_completion.d/cpoi
_cpoi_completions() {
    local cur prev opts commands

    # Current word being completed
    cur="${COMP_WORDS[COMP_CWORD]}"
    
    # Previous word
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    # Command options and commands
    opts="-v --version -c --config"
    commands="c uc p d"

    # Complete option flags or commands
    if [[ "${COMP_CWORD}" -eq 1 ]]; then
        COMPREPLY=( $(compgen -W "${opts} ${commands}" -- "${cur}") )
        return 0
    fi

    # Complete for --config or -c
    if [[ "${prev}" == "-c" || "${prev}" == "--config" ]]; then
        COMPREPLY=( $(compgen -W "<key>" -- "${cur}") )
        return 0
    fi

    # After providing a config key, suggest value
    if [[ "${COMP_WORDS[COMP_CWORD-2]}" == "-c" || "${COMP_WORDS[COMP_CWORD-2]}" == "--config" ]]; then
        COMPREPLY=( $(compgen -W "<value>" -- "${cur}") )
        return 0
    fi

    # Complete for commands [c, uc, p, d]
    if [[ "${prev}" == "c" || "${prev}" == "uc" || "${prev}" == "p" || "${prev}" == "d" ]]; then
        COMPREPLY=( $(compgen -W "<value>" -- "${cur}") )
        return 0
    fi
}

complete -F _cpoi_completions ./cpoi
