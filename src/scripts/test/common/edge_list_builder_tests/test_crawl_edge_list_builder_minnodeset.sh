#!/bin/bash -eu

trap 'echo "E: Crawl edge list builder test failed with exitcode $?"' ERR

this_dir="$(dirname "$0")"
input=$this_dir/../../../../../resources/edge_list_builder_test/graph2
expectedCrawl=$this_dir/../../../../../resources/edge_list_builder_test/expectedCrawl2
maxNodeToKeep=4
minnode=3

work_dir=`mktemp -d`
output=$work_dir/outputGraph

$this_dir/../../../../bin/test/useFileSystem/common/edge_list_builder_tests/crawl_edge_list_builder_main $maxNodeToKeep  $input $output $minnode
output_diff=`diff $expectedCrawl $output`

if [ "$output_diff" != "" ]; then
  echo Crawl edge list builder test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Crawl edge list builder test accepted."

exit 0
