#!/bin/bash -u

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/gsl

mkdir -p $INSTALL_DIR
tempdir=`mktemp -d`

cd $tempdir

wget http://ftp.gnu.org/pub/gnu/gsl/gsl-1.16.tar.gz
tar -zxvf gsl-1.16.tar.gz
cd gsl-1.16

./configure --prefix="$INSTALL_DIR" --disable-static
make -j4
make -j4 html
make -j4 install

rm -rf "$tempdir"
