#!/bin/bash -u

trap "echo Some errors occurred while installing gmock. Exiting." ERR

SCRIPT_DIR=$(dirname "$0")
INSTALL_DIR=$SCRIPT_DIR/../../../../dep/gmock/
DEP_DIR=$SCRIPT_DIR/../../../../dep/

if [ -d "$INSTALL_DIR" ]; then
  echo "$INSTALL_DIR exists already. Exiting."
  exit 0
fi

tempdir=`mktemp -d`
trap "rm -rf $tempdir" EXIT

cd $tempdir

wget http://googlemock.googlecode.com/files/gmock-1.6.0.zip

unzip gmock-1.6.0.zip
mv gmock-1.6.0 $DEP_DIR/gmock

cd $INSTALL_DIR

GMOCK_DIR=`pwd`
GTEST_DIR="$GMOCK_DIR"/gtest

mkdir build
cd build

g++ -I${GTEST_DIR}/include -I${GTEST_DIR} -I${GMOCK_DIR}/include \
  -I${GMOCK_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
g++ -I${GTEST_DIR}/include -I${GTEST_DIR} -I${GMOCK_DIR}/include \
  -I${GMOCK_DIR} -c ${GMOCK_DIR}/src/gmock-all.cc
ar -rv libgmock.a gtest-all.o gmock-all.o

echo "GMOCK installed successfully."
