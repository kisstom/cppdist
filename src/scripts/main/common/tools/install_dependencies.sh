#!/bin/bash -eu

trap "echo Some errors occurred while installing dependencies. Exiting." ERR

SCRIPT_DIR=$(readlink -f $(dirname "$0"))

SCRIPT_DIR/install_gmock.sh
SCRIPT_DIR/install_gmp.sh
SCRIPT_DIR/install_gsl.sh
SCRIPT_DIR/install_gtest.sh
SCRIPT_DIR/install_logcpp.sh 
SCRIPT_DIR/install_zmq.sh
