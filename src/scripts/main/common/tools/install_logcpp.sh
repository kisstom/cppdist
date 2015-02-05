#!/bin/bash -u

trap "echo Some errors occurred while installing log4cpp. Exiting." ERR

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/log4cpp

if [ -d "$INSTALL_DIR" ]; then
  echo "$INSTALL_DIR exists already. Exiting."
  exit 0
fi

mkdir -p $INSTALL_DIR
tempdir=`mktemp -d`

trap "rm -rf $tempdir" EXIT

cd $tempdir

wget http://sourceforge.net/projects/log4cpp/files/log4cpp-1.1.1.tar.gz
tar -zxvf log4cpp-1.1.1.tar.gz
cd log4cpp

./configure --prefix="$INSTALL_DIR" 
make -j4
make -j4 install

echo "Log4cpp installed successfully."
