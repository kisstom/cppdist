#!/bin/bash -eu

thisDir=$(dirname $0)
. $thisDir/test_dir.sh


cd $thisDir/../../../../cpp
scons -j4


cd ..
test_dir ./bin/test/common

#./bin/test/algos/simrank_update/test_node


if [ $# != 1 ]; then
  echo "Setting resource path to ../resources/"
  resourcePath="../resources/"
else
  resourcePath=${1}
fi

test_dir ./bin/test/useFileSystem/ $resourcePath

echo "Hash function test."
./scripts/test/simrank/test_cpp_java_hash.sh

test_dir ./bin/test/algos/simrank_odd_even/
test_scripts ./scripts/test/

echo "File system tests passed."
echo Successfully done building and testing.
