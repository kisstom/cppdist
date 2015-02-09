#!/bin/bash -eu

trap "echo Test failed." ERR

echo "------------------------ Starting index computer test ------------------------"

thisDir=$(readlink -f $(dirname $0))
toolDir=../../../../bin/main/common/tools/
resourceDir=../../../../../resources/hash_compute_test/

tmpDir=`mktemp -d`
trap "rm -r $tmpDir" EXIT

for i in `seq 0 3`
do 
  $toolDir/outpartition_hash_computer_job $resourceDir/part/slavery_$i.txt $resourceDir/part/slavery.cfg 4 1000 $i $tmpDir/hash $tmpDir/flag > /dev/null
  
  diff $resourceDir/expected/"$i"_hash.txt $tmpDir/hash > /dev/null
  diff $resourceDir/expected/"$i"_flag.txt $tmpDir/flag > /dev/null
done

echo "Test passed."
