this_dir="$(dirname "$0")"
input=$this_dir/../../../../../resources/edge_list_builder_test/graph
filterNodeFile=$this_dir/../../../../../resources/edge_list_builder_test/filterNode

work_dir=`mktemp -d`
output=$work_dir/outputGraph

$this_dir/../../../../bin/test/useFileSystem/common/edge_list_builder_tests/filter_edge_list_builder $filterNodeFile  $input $output 
output_diff=`diff $input $output`

if [ "$output_diff" != "" ]; then
  echo Filter edge list build test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Edge list builder test accepted."
exit 0
