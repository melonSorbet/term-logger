#!/bin/bash
if [[ -z "$INSIDE_SCRIPT" ]]; then
    export INSIDE_SCRIPT=1
    date=$(date '+%d')
    hour=$(date -u '+%H:%M:%S')
    monthandday=$(date '+%Y-%m')
    echo "$hour"

    mkdir -p "$HOME/.local/state/term-logger/$monthandday/"
    loglocation="$HOME/.local/state/term-logger/$monthandday/$date-$hour.log"

    script -f -q "$loglocation"

    (
    trap '' INT TERM  # ignore Ctrl+C

    tmpfile=$(mktemp)

    # add tmux plugin / configuration to execute this. otherwise it will be killed without finishing the job
    # and catch the CTRL C and exit
    ansi2txt -w 5000000 < "$loglocation" | col -b > "$tmpfile" && mv "$tmpfile" "$loglocation"
) &
    exit
fi
