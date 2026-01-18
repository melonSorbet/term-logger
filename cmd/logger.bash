if [[ -z "$INSIDE_SCRIPT" ]]; then
    export INSIDE_SCRIPT=1
    date=$(date '+%d')
    hour=$(date -u '+%H:%M:%S')
    monthandday=$(date '+%Y-%m')
    echo "$hour"
  
    mkdir -p "$HOME/.local/state/term-logger/$monthandday/"
    loglocation="$HOME/.local/state/term-logger/$monthandday/$date-$hour.log"
    
    script -f -q "$loglocation"
    
    # Cleanup
     (
        trap '' INT TERM  # ignore Ctrl+C and kill signals
        tmpfile=$(mktemp)
        
        sed -E '
            s/\x1b\[[?][0-9;]*[a-zA-Z]//g;
            s/\x1b\[[0-9;]*[mKHJfSTABCDEFGsu]//g;
            s/\x1b\][0-9]*;[^\x07]*\x07//g;
            s/\x1b\][0-9]*;[^\x1b]*\x1b\\//g;
            s/\x1b[=>]//g;
            s/\x0f//g;
            s/\x0e//g;
            1,2d
        ' "$loglocation" | tr -d '\r' > "$tmpfile"
        
        mv "$tmpfile" "$loglocation"
    ) &    
    exit
fi
