#!/bin/bash -eu

trap 'echo "E: Socket test failed with exitcode $?"' ERR

echo Starting socket test
this_dir="$(dirname "$0")"

work_dir=`mktemp -d`

port=6000
output=$work_dir/client_received
expected_output=$work_dir/expected_output

echo -e "hava\nnagila\njava\n\nvege" > $expected_output

$this_dir/../../../../bin/test/useFileSystem/common/socket/server_main $port "hava,nagila,java,,vege" &
$this_dir/../../../../bin/test/useFileSystem/common/socket/client_main $port $output 

output_diff=`diff $expected_output $output`

if [ "$output_diff" != "" ]; then
  echo Socket test failed. Diff:
  echo $output_diff
  exit 1
fi

rm -rf "$work_dir"

echo "Socket test accepted."

exit 0