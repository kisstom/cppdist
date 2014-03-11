this_dir="$(dirname "$0")"
input=$this_dir/../../../../../resources/crawl_test/part2
#expected_file=$this_dir/../../../../../resources/crawl_test/expectedEdgeListPart2

work_dir=`mktemp -d`
output=$work_dir/outputPart

minnode=6
numnode=10
nextMinnode=16

$this_dir/../../../../bin/test/useFileSystem/common/edge_list_builder_tests/edge_list_builder_with_simrank $input $output $minnode $numnode $nextMinnode 

output_diff=`diff $input $output`

if [ "$output_diff" != "" ]; then
  echo Filter test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Edge list builder test accepted."
exit 0
