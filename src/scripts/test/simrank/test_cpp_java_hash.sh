#!/bin/bash -eu

exit 0

echo "test java cpp hash"

this_dir="$(dirname "$0")"
resource_dir=$this_dir/../../../../resources/pseudo_tests

random_files=`ls $resource_dir/random_*`
work_dir=`mktemp -d`

for file in $random_files
do
  seed=`echo $file| sed 's/.*random_//'`
  file_pref=`echo $file| sed 's/.*\///'`
  $this_dir/../../../bin/test/shtest/pseudo_random_test $file $work_dir/$file_pref.cppout $seed
  java -cp $this_dir/../../../dep/java/hadoop.jar hu.sztaki.ilab.cumulonimbus.hadoop.PseudoRandom $file $work_dir/$file_pref.javaout $seed
  output_diff=`diff $work_dir/$file_pref.cppout $work_dir/$file_pref.javaout`
  if [ "$output_diff" != "" ]; then
    echo Test for file $file failed. Diff:
    echo $output_diff
    exit 1
  fi
done

rm -rf "$work_dir"
exit 0
