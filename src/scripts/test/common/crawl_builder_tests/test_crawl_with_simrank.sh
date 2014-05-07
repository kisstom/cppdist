#!/bin/bash -eu

this_dir="$(dirname "$0")"
input=$this_dir/../../../../../resources/crawl_test/part2
expected_file=$this_dir/../../../../../resources/crawl_test/expectedPart2

work_dir=`mktemp -d`
output=$work_dir/outputPart

minnode=6
numnode=10
maxnodeToKeep=11
nextMinnode=16

$this_dir/../../../../bin/test/useFileSystem/common/edge_list_builder_tests_with_algo/crawl_edge_list_builder_with_simrank $input $output $minnode $numnode $nextMinnode $maxnodeToKeep 

output_diff=`diff $expected_file $output`

if [ "$output_diff" != "" ]; then
  echo Filter test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Crawl test accepted."
exit 0
