#!/bin/bash -eu

echo -e "Starting out partition indexer test.\n-----------------------------------------\n"

trap 'rm -rf "$work_dir"' EXIT

this_dir="$(dirname "$0")"
resourceDir="$this_dir"/../../../../../resources/partitioner_tests/
work_dir=`mktemp -d`

inputPartition="$resourceDir"/split/slavery_1.txt
config="$resourceDir"/slavery.cfg

outNeighbors="$work_dir"/nn.txt
outPartitionIndices="$work_dir"/opi.txt

expectedNeghbors=$resourceDir/expected_partition_indices/nn_1.txt
expectedOutPartitionIndices=$resourceDir/expected_partition_indices/opi_1.txt

"$this_dir"/../../../../bin/main/common/tools/outpartition_index_computer_job \
"$inputPartition" "$config" 4 10000000 4 $outNeighbors $outPartitionIndices

diff $expectedNeghbors $outNeighbors
diff $expectedOutPartitionIndices $outPartitionIndices

echo -e "-----------------------------------------\nPASSED.\n"