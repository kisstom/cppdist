#!/bin/bash -eu

if [ "$#" -lt 3 ]; then
  echo "Wrong number of params. Correct: input numParts outputDir startFromZero"
  exit
fi

input="$1"
numParts="$2"
outputDir="$3"
startFromZero="0"

if [ "$#" -gt 3 ]; then
  startFromZero=$4
fi

echo "start from zero $startFromZero"

rm -rf "$outputDir"
mkdir -p "$outputDir"

thisDir=$(dirname $0)
thisDir=$(readlink -f $thisDir)

srcdir=$thisDir/../../../../../

date
echo "counting nodes..."
numNodes=`awk -v startFromZero="$startFromZero" \
'BEGIN{max=0} {if ($1 > max) max = $1; if ($2 > max) max = $2}\
END{if (startFromZero == 1) ++max; print max}' "$input"`
echo "number of nodes: $numNodes" 

date
echo "sorting edge list..."
sort -T "$HOME" -gk1,1 "$input" > "$outputDir"/sorted_edgelist.txt

date
echo "shuffle continue rendexing..."
"$srcdir"/bin/main/common/tools/shuffle_continue_reindexer_job "$outputDir"/sorted_edgelist.txt "$outputDir"/shuffle_continue_reindexed_graph\
  "$outputDir"/reindexed_hash $numNodes $numParts

date
echo "sorting new graph..."
sort -T "$HOME" -gk1,1 "$outputDir"/shuffle_continue_reindexed_graph > "$outputDir"/shuffle_continue_reindexed.sort


date
echo "creating clue format..."
"$srcdir"/bin/main/common/graph_converter/print_clue_format "$outputDir"/shuffle_continue_reindexed.sort "$outputDir"/reindexed_graph.clue "$numNodes"
