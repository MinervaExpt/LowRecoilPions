
if [ "$BASH" ]; then
if [[ $- == *i* ]]; then

_samweb_completion() {
    local c cmd_seen cur cmds
    COMPREPLY=()
    cur="${2}"
    cmds=$(samweb list-cli-commands 2> /dev/null)
    cmd_seen=0
    # check to see if we've found a sub-command yet
    for c in ${COMP_WORDS[@]}; do
        if [[ " $cmds" =~ [[:space:]]$c[[:space:]] ]]; then
            cmd_seen=1
            # handle case where this is the entire name; then it's the only match
            if [ $c = "$cur" ]; then
                COMPREPLY=("$cur")
                return 0;
            fi
            break
        fi
    done
    case "$cur" in
        -*)
            COMPREPLY=()
            ;;
        *)
            if [ $cmd_seen = 1 ]; then
                # if we've already seen a command there's not much we can do
                # may as well supply file names
                COMPREPLY=($(compgen -f -- ${cur}))
            else
                COMPREPLY=($(compgen -W "${cmds}" -- ${cur}))
            fi
    esac
    return 0
}

complete -F _samweb_completion samweb

fi
fi
