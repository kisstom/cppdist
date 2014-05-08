#!/bin/bash -eu

trap 'echo "E: Filter edge list builder test failed with exitcode $?"' ERR

this_dir="$(dirname "$0")"
input=$this_dir/../../../../../resources/edge_list_builder_test/graph
filterNodeFile=$this_dir/../../../../../resources/edge_list_builder_test/filterNode
expectedFiltered=$this_dir/../../../../../resources/edge_list_builder_test/expectedFiltered
minnode=10

work_dir=`mktemp -d`
output=$work_dir/outputGraph

$this_dir/../../../../bin/test/useFileSystem/common/edge_list_builder_tests/filter_edge_list_builder_main $filterNodeFile  $input $output $minnode
output_diff=`diff $expectedFiltered $output`

if [ "$output_diff" != "" ]; then
  echo Filter edge list build test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Filter edge list builder test accepted."
exit 0
