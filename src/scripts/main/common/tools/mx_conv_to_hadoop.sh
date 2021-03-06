thisDir=$(dirname $0)

if [ "$#" -ne 3 ]; then
  echo "usage: input graf num_nodes output"
  exit 1
fi

input=$1
numNodes=$2
output=$3

tmp_output=`mktemp`
$thisDir/../../../../bin/main/common/graph_converter/inverse_job $input $numNodes $tmp_output

awk -f "$thisDir"/add_start_node_index.awk $tmp_output > $output

rm "$tmp_output"
