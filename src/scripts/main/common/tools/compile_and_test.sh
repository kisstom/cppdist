thisDir=$(dirname $0)
. $thisDir/test_dir.sh


cd $thisDir/../../../../cpp
scons -j4

if [ $? != 0 ]; then
  echo Build failed. Exiting.
  exit
fi

cd ..
for x in `find ./bin/test/common -name *test* |grep -v ".*\.o"` 
do
  if [ -d $x ]; then
    continue
  fi
    
  $x
  if [ $? != 0 ]; then
    echo Test failed. Exiting.
    exit
  fi
  echo -e "\n\n--------------------------------------------------"
done

#./bin/test/algos/simrank_update/test_node

./bin/test/algos/test_cluster_setup
if [ $? != 0 ]; then
  echo Test failed. Exiting.
  exit
fi



if [ $# != 1 ]; then
  echo "Setting resource path to ../resources/"
  resourcePath="../resources/"
else
  resourcePath=${1}
fi

fsTestFiles=`find  ./bin/test/useFileSystem/* -name "*test*"| grep -v ".*\.o"`

for x in $fsTestFiles
do
  if [ -d $x ]; then
    continue
  fi

  $x $resourcePath
  if [ $? != 0 ]; then
    echo $x test failed. Exiting.
    exit
  fi
  echo -e "\n\n--------------------------------------------------"

done

echo "Hash function test."
./scripts/test/simrank/test_cpp_java_hash.sh
if [ $? != 0 ]; then
  echo Test failed. Exiting.
  exit
fi


test_dir ./bin/test/algos/simrank_odd_even/
test_scripts ./scripts/test/

echo "File system tests passed."
echo Successfully done building and testing.
