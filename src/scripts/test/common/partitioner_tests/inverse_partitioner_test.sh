#!/bin/bash -eu

echo -e "Starting inverse partitioner test.\n-----------------------------------------\n"

trap 'rm -rf "$work_dir"' EXIT

this_dir="$(dirname "$0")"
resourceDir="$this_dir"/../../../../../resources/partitioner_tests/
input="$resourceDir"/graph.txt

work_dir=`mktemp -d`

for i in `seq 0 3`
do
	mkdir "$work_dir"/part_"$i"	
done


"$this_dir"/../../../../bin/main/common/tools/inverse_partition_maker_job \
"$input" "$work_dir" "$resourceDir"/slavery.cfg 4 10000000


for i in `seq 0 3`
do
	diff "$work_dir"/part_"$i"/edges.txt "$resourceDir"/expected_inverse/part_"$i"/edges.txt
	diff "$work_dir"/part_"$i"/bound.txt "$resourceDir"/expected_inverse/part_"$i"/bound.txt
	echo $i
done

echo -e "-----------------------------------------\nPASSED.\n"