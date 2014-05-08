#!/bin/bash -eu

trap 'echo "E: Edge list builder test failed with exitcode $?"' ERR

this_dir="$(dirname "$0")"
input=$this_dir/../../../../../resources/edge_list_builder_test/graph

work_dir=`mktemp -d`
output=$work_dir/outputGraph
minnode=20

$this_dir/../../../../bin/test/useFileSystem/common/edge_list_builder_tests/edge_list_builder_main $input $output $minnode
output_diff=`diff $input $output`

if [ "$output_diff" != "" ]; then
  echo Edge list builder test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Edge list builder test accepted."
exit 0
