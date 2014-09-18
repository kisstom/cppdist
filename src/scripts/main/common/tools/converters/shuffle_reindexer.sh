#!/bin/bash -eu

if [ "$#" != 3 ]; then
	echo "Wrong number of params."
fi

input="$1"
partNum="$2"
outputDir="$3"

rm -rf "$outputDir"
mkdir -p "$outputDir"

thisDir=$(dirname $0)
srcdir=$thisDir/../../../../../
awk -v partNum="$partNum" -f "$thisDir"/part_index_computer.awk $input > "$outputDir"/part_indices.txt

numnodes=`wc -l $outputDir/part_indices.txt|cut -f1 -d' '`
echo "number of nodes: " $numnodes

sort -T /home/kisstom/ -gk1,1 -gk2,2 "$outputDir"/part_indices.txt > "$outputDir"/part_indices.sort
$srcdir/bin/main/common/tools/shuffle_reindexer_job $input "$outputDir"/shuffle_reindex "$outputDir"/part_indices.sort

sort -T /home/kisstom/ -gk1,1 $outputDir/shuffle_reindex > $outputDir/shuffle_reindex.sort
echo "sort done"
date

$srcdir/bin/main/common/graph_converter/print_clue_format $outputDir/shuffle_reindex.sort $outputDir/shuffle_reindex.clue $numnodes
echo "clue format created"
date
