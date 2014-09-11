#!/bin/bash -eu

SCRIPT_DIR=$(dirname "$0")
INSTALL_DIR=$SCRIPT_DIR/../../../../dep
#INSTALL_DIR=/home/kisstom/sandbox/

mkdir -p $INSTALL_DIR
cd $INSTALL_DIR

wget http://googletest.googlecode.com/files/gtest-1.6.0.zip

unzip gtest-1.6.0.zip
mv gtest-1.6.0 gtest

cd gtest

GTEST_DIR=`pwd`

mkdir build
cd build

g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
ar -rv libgtest.a gtest-all.o

echo "GTEST successsfulle installed in $GTEST_DIR" 
