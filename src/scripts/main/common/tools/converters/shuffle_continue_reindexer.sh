#!/bin/bash -eu

if [ "$#" != 4 ]; then
  echo "Wrong number of params. Correct: input numParts outputDir numNodes"
  exit
fi

input="$1"
numParts="$2"
outputDir="$3"
numNodes="$4"

rm -rf "$outputDir"
mkdir -p "$outputDir"

thisDir=$(dirname $0)
thisDir=$(readlink -f $thisDir)

srcdir=$thisDir/../../../../../

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
