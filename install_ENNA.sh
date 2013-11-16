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
echo "ENNA_HELP:    ${ENNA_HELP:?}"
echo "ENNA_ARCHIVE: ${ENNA_ARCHIVE:?}"
echo "ENNA_REPORTS: ${ENNA_REPORTS:?}"

if ! id -u $ENNA_USER > /dev/null 2>&1; then
    # add user without home dir
    adduser --no-create-home --disabled-password --gecos "" $ENNA_USER
fi

[ -d $ENNA_PATH ] || mkdir -p $ENNA_PATH
[ -d $ENNA_HELP ] || mkdir -p $ENNA_HELP
[ -d $ENNA_ARCHIVE ] || mkdir $ENNA_ARCHIVE
[ -d $ENNA_REPORTS ] || mkdir $ENNA_REPORTS

cp HELP/*.jpg $ENNA_HELP
cp Help_for_ENNA.html $ENNA_HELP/index.html
cp _MAIN_PAGE_.html $ENNA_ARCHIVE
cp _MAIN_PAGE_.html $ENNA_REPORTS


chown --recursive $ENNA_USER:$ENNA_USER $ENNA_PATH

sudo -u $ENNA_USER bash << EOF
cd $ENNA_PATH
chmod 777 $ENNA_PATH $ENNA_ARCHIVE $ENNA_REPORTS
chmod 700 $ENNA_HELP
nohup python -m SimpleHTTPServer $ENNA_PORT > /dev/null 2>&1 &
EOF

echo
echo "Check running ENNA server at http://"`hostname  -I | cut -f1 -d' '`":$ENNA_PORT"
