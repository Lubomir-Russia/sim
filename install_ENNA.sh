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

echo "ENNA_USER:    ${ENNA_USER:?}"
echo "ENNA_PORT:    ${ENNA_PORT:?}"
echo "ENNA_PATH:    ${ENNA_PATH:?}"
echo "ENNA_ARCHIVE: ${ENNA_ARCHIVE:?}"

if ! id -u $ENNA_USER > /dev/null 2>&1; then
    # add user without home dir
    adduser --no-create-home --disabled-password --gecos "" $ENNA_USER
fi

[ -d $ENNA_PATH ] || mkdir -p $ENNA_PATH
chown $ENNA_USER:$ENNA_USER $ENNA_PATH

sudo -u $ENNA_USER bash << EOF
[ -d $ENNA_ARCHIVE ] || mkdir $ENNA_ARCHIVE
cp Help_for_ENNA.html $ENNA_PATH
cd $ENNA_PATH
nohup python -m SimpleHTTPServer $ENNA_PORT > /dev/null 2>&1 &
EOF

echo
echo "Check running ENNA server at http://"`hostname  -I | cut -f1 -d' '`":$ENNA_PORT"
