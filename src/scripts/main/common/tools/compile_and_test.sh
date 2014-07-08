#!/bin/bash -eu

set -o pipefail

thisDir=$(dirname $0)
. $thisDir/test_dir.sh


cd $thisDir/../../../../cpp
scons -j4


cd ..
echo common tests
test_dir ./bin/test/common

echo dmoz tests
test_dir ./bin/test/dmoz

#./bin/test/algos/simrank_update/test_node


if [ $# != 1 ]; then
  echo "Setting resource path to ../resources/"
  resourcePath="../resources/"
else
  resourcePath=${1}
fi

echo filesystem tests
test_dir ./bin/test/useFileSystem/ $resourcePath

echo algo tests
test_dir ./bin/test/algos/simrank_odd_even/
test_dir ./bin/test/algos/psimrank/

echo script tests
test_scripts ./scripts/test/

echo Successfully done building and testing.
