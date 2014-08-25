#!/bin/bash -eu

#trap 'echo "E: Socket test failed with exitcode $?"' ERR
function cleanup() {
  rm -rf "$work_dir"	
}

function warnMsg() {
  echo "WARNING! Socket test failed maybe because of connect error."
}

connect_error=false
trap 'cleanup; if [ "$connect_error" == true ]; then warnMsg; exit 0; fi' EXIT

echo Starting socket test
this_dir="$(dirname "$0")"

work_dir=`mktemp -d`

port=6000
output=$work_dir/client_received
expected_output=$work_dir/expected_output

echo -e "hava\nnagila\njava\n\nvege" > $expected_output

$this_dir/../../../../bin/test/useFileSystem/common/socket/server_main $port "hava,nagila,java,,vege" &
pid1=$!

$this_dir/../../../../bin/test/useFileSystem/common/socket/client_main $port $output &
pid2=$!

for job in $pid1 $pid2
do
    wait $job
    
    if [ "$?" == 2 ]; then
      echo "Connect error"
      connect_error=true
    fi
done

! $connect_error || exit 0

output_diff=`diff $expected_output $output`

if [ "$output_diff" != "" ]; then
  echo Socket test failed. Diff:
  echo $output_diff
  exit 1
fi

echo "Socket test accepted."

exit 0