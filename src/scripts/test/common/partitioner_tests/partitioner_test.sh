#!/bin/bash -eu

echo -e "Starting partitioner test.\n-----------------------------------------\n"

trap 'rm -rf "$work_dir"' EXIT

this_dir="$(dirname "$0")"
resourceDir="$this_dir"/../../../../../resources/partitioner_tests/
input="$resourceDir"/graph.txt

work_dir=`mktemp -d`

"$this_dir"/../../../../bin/main/common/tools/split_clueweb_vector_node \
-numedge_perpart 4 -init_slave_port 6000  \
-part_prefix $work_dir/slavery \
-input $input \
-partition_cfg $work_dir/slavery.cfg

for i in `seq 0 3`
do
	df=`diff $work_dir/slavery_"$i".txt "$resourceDir"/split/slavery_"$i".txt`
done

echo -e "-----------------------------------------\nPASSED.\n"