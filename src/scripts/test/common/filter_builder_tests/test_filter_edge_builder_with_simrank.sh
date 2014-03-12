this_dir="$(dirname "$0")"
input=$this_dir/../../../../../resources/filter_test/small.txt
expected_file=$this_dir/../../../../../resources/filter_test/expected_to_simrank

work_dir=`mktemp -d`
output=$work_dir/outputPart

minnode=6
numnode=10
nextMinnode=16
nodesToFilter=$this_dir/../../../../../resources/filter_test/nodes_to_remove_simrank

$this_dir/../../../../bin/test/useFileSystem/common/edge_list_builder_tests/filter_edge_list_builder_with_simrank $input $output $minnode $numnode $nextMinnode $nodesToFilter

output_diff=`diff $expected_file $output`

if [ "$output_diff" != "" ]; then
  echo Filter test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Filter test accepted."
exit 0
