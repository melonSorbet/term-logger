if [ $# -ne 1 ]; then
    exit 1
fi
loglocation=$1
tmp="$loglocation.tmp"
sed -E '
    s/\x1b\[[?][0-9;]*[a-zA-Z]//g;
    s/\x1b\[[0-9;]*[mKHJfSTABCDEFGsu]//g;
    s/\x1b\][0-9]*;[^\x07]*\x07//g;
    s/\x1b\][0-9]*;[^\x1b]*\x1b\\//g;
    s/\x1b[=>]//g;
    s/\x0f//g;
    s/\x0e//g;
    1,2d;
    $d
' "$loglocation" | tr -d '\r' > "$tmp"

