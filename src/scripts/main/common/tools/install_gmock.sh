#!/bin/bash -eu

SCRIPT_DIR=$(dirname "$0")
INSTALL_DIR=$SCRIPT_DIR/../../../../dep

mkdir -p $INSTALL_DIR
cd $INSTALL_DIR

wget http://googlemock.googlecode.com/files/gmock-1.6.0.zip

unzip gmock-1.6.0.zip
mv gmock-1.6.0 gmock

cd gmock

GMOCK_DIR=`pwd`
GTEST_DIR="$GMOCK_DIR"/gtest

mkdir build
cd build

g++ -I${GTEST_DIR}/include -I${GTEST_DIR} -I${GMOCK_DIR}/include \
  -I${GMOCK_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
g++ -I${GTEST_DIR}/include -I${GTEST_DIR} -I${GMOCK_DIR}/include \
  -I${GMOCK_DIR} -c ${GMOCK_DIR}/src/gmock-all.cc
ar -rv libgmock.a gtest-all.o gmock-all.o
