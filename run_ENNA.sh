#!/bin/bash

if [ $EUID -ne 0 ]; then
    echo "This script must be run as root, use sudo." 1>&2
    exit 1
fi

ENNA_env=./ENNA_env.sh
if [ ! -r $ENNA_env ]; then
    echo "Cannot read file $ENNA_env with ENNA enviroment settings." 1>&2
    exit 1
fi
. $ENNA_env

echo "Restarting ENNA server in ${ENNA_PATH:?}"
sudo -u ${ENNA_USER:?} bash << EOF
cd ${ENNA_PATH:?}
nohup python -m SimpleHTTPServer ${ENNA_PORT:?} > /dev/null 2>&1 &
EOF

echo
echo "Check running ENNA server at http://"`hostname  -I | cut -f1 -d' '`":${ENNA_PORT:?}"
