#!/bin/bash -u 

trap "echo Some errors occurred while installing gsl. Exiting." ERR

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/gsl

if [ -d "$INSTALL_DIR" ]; then
  echo "$INSTALL_DIR exists already. Exiting."
  exit 0
fi

mkdir -p $INSTALL_DIR
tempdir=`mktemp -d`

trap "rm -rf $tempdir" EXIT

cd $tempdir

wget http://ftp.gnu.org/pub/gnu/gsl/gsl-1.16.tar.gz
tar -zxvf gsl-1.16.tar.gz
cd gsl-1.16

./configure --prefix="$INSTALL_DIR" --disable-static
make -j4
make -j4 html
make -j4 install

echo "GSL installed successfully."
