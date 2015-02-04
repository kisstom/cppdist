#!/bin/bash -u

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/log4cpp

mkdir -p $INSTALL_DIR
tempdir=`mktemp -d`

cd $tempdir

wget http://sourceforge.net/projects/log4cpp/files/log4cpp-1.1.1.tar.gz
tar -zxvf log4cpp-1.1.1.tar.gz
cd log4cpp

./configure --prefix="$INSTALL_DIR" 
make -j4
make -j4 install

rm -rf "$tempdir"
