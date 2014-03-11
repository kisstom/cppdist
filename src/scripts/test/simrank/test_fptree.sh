thisDir="$(cd "$(dirname $0)" && pwd)"

hadoop_dir=/home/kisstom/git/cumulonimbus/repo/hadoop-0.20.2/hadoop-0.20.2
classname="hu.sztaki.SortedTreeCompute.SortedTreeComputer"

inputFSPyth=$thisDir/../../../../resources/small_no_loop.path10.sort
inputFSHad=$thisDir/../../../../resources/small_no_loop.path10.sort.rootback

inputHad=file:///$inputFSHad

echo $input
outputFS=$thisDir/tmp.fptree
output=file:///$outputFS

"$hadoop_dir"/bin/hadoop jar $thisDir/../../../dep/java/FingerprintTree.jar $classname $inputHad $output

cat $outputFS/part* > $outputFS/hadoop.out

python $thisDir/../../main/simrank/compute_tree.py $inputFSPyth $outputFS/python.out

output_diff=`diff $outputFS/hadoop.out $outputFS/python.out`

if [ "$output_diff" != "" ]; then
  echo Test for fptree count failed. Diff:
  #echo $output_diff
  exit 1
fi

echo "FP tree counter test passed."
rm -rf "$outputFS"
exit 0
