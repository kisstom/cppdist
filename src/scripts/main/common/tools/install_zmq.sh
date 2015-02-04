#!/bin/bash -u

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/zmq

mkdir -p $INSTALL_DIR
tempdir=`mktemp -d`

cd $tempdir

wget http://download.zeromq.org/zeromq-4.0.5.tar.gz
tar -zxvf zeromq-4.0.5.tar.gz
cd zeromq-4.0.5

./configure --prefix="$INSTALL_DIR"
make -j4
make -j4 install

rm -rf "$tempdir"
