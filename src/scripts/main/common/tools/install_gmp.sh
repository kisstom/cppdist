#!/bin/bash -u

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/gmp

mkdir -p $INSTALL_DIR
tempdir=`mktemp -d`

cd $tempdir

wget http://ftp.gnu.org/gnu/gmp/gmp-5.1.3.tar.gz 
tar -zxvf gmp-5.1.3.tar.gz 
cd gmp-5.1.3/

./configure --prefix="$INSTALL_DIR"
make -j4
make -j4 install

rm -rf "$tempdir"
