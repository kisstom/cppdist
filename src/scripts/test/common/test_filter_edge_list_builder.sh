this_dir="$(dirname "$0")"
small_matrix=$this_dir/../../../../resources/filter_test/small.txt
nodes_to_remove=$this_dir/../../../../resources/filter_test/nodes_to_remove
expected_file=$this_dir/../../../../resources/filter_test/expected_file

work_dir=`mktemp -d`
small_matrix_filtered=$work_dir/small_matrix_filtered

$this_dir/../../../bin/main/common/tools/node_remover $small_matrix $small_matrix_filtered $nodes_to_remove
output_diff=`diff $expected_file $small_matrix_filtered`

if [ "$output_diff" != "" ]; then
  echo Filter test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Filter test accepted."

exit 0


