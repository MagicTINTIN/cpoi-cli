# ~/.zsh/completion/_cpoi
#compdef cpoi

_cpoi() {
  _arguments \
    '-v[display version]' \
    '--version[display version]' \
    '-c[set config key value]:key: :value:' \
    '--config[set config key value]:key: :value:' \
    '1:command:(c uc p d)' \
    '2:value:'
}

compdef _cpoi cpoi
