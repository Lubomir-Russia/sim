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

./enna stop

timestamp=$( date +"%Y-%m-%d_%H-%M" )
ENNA_BACKUP=$ENNA_PATH"/../Back_up_ENNA_-_"$timestamp

[ -d $ENNA_PATH ] && mv $ENNA_PATH $ENNA_BACKUP
