#!/bin/bash -u

trap "echo Some errors occurred while installing zmq. Exiting." ERR

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/zmq

if [ -d "$INSTALL_DIR" ]; then
  echo "$INSTALL_DIR exists already. Exiting."
  exit 0
fi

mkdir -p $INSTALL_DIR
tempdir=`mktemp -d`

trap "rm -rf $tempdir" EXIT

cd $tempdir

wget http://download.zeromq.org/zeromq-4.0.5.tar.gz
tar -zxvf zeromq-4.0.5.tar.gz
cd zeromq-4.0.5

./configure --prefix="$INSTALL_DIR"
make -j4
make -j4 install

echo "ZMQ installed successfully."
