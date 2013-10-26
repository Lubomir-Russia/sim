#!/bin/bash

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root, use sudo." 1>&2
    exit 1
fi

echo "ENNA_PATH: " $ENNA_PATH
. ./ENNA_env.sh
echo "ENNA_PATH: " $ENNA_PATH
exit 0

if ! id -u $ENNA_USER > /dev/null 2>&1; then
    adduser $ENNA_USER
fi

[ -d $ENNA_PATH ] || mkdir -p $ENNA_PATH
chown $ENNA_USER:$ENNA_USER $ENNA_PATH

sudo -u $ENNA_USER bash << EOF
[ -d $ENNA_ARCHIVE ] || mkdir $ENNA_ARCHIVE
cp Help_for_ENNA.html $ENNA_PATH
cd $ENNA_PATH
nohup python -m SimpleHTTPServer $ENNA_PORT > /dev/null 2>&1 &
EOF
