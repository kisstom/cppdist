#!/bin/bash -eu

trap "echo Some errors occurred while installing gmp. Exiting." ERR

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/gmp

if [ -d "$INSTALL_DIR" ]; then
  echo "$INSTALL_DIR exists already. Exiting."
  exit 0
fi

mkdir -p $INSTALL_DIR
tempdir=`mktemp -d`

trap "rm -rf $tempdir" EXIT

cd $tempdir

wget http://ftp.gnu.org/gnu/gmp/gmp-5.1.3.tar.gz 
tar -zxvf gmp-5.1.3.tar.gz 
cd gmp-5.1.3/

./configure --prefix="$INSTALL_DIR"
make -j4
make -j4 install

echo "GMP installed successfully."
