this_dir="$(dirname "$0")"
small_matrix=$this_dir/../../../../resources/filter_test/small.txt
work_dir=`mktemp -d`
small_matrix_out=$work_dir/small.txt

$this_dir/../../../bin/test/useFileSystem/common/edge_list_builder_tool $small_matrix $small_matrix_out
output_diff=`diff $small_matrix $small_matrix_out`

if [ "$output_diff" != "" ]; then
  echo Filter test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Filter test accepted."

exit 0


