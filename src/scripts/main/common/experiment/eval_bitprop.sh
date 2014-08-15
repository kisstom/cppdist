#!/bin/bash -eu

if [ "$#" != 6 ]; then
  echo usage: outdir maxDistance sampleSize maxNodeId graph experimentDir
fi

thisDir=$(dirname $0)

outdir=$1
maxdist=$2
sampleSize=$3
maxNodeId=$4
graph=$5
experimentDir=$6

rm -rf "$outdir"
mkdir "$outdir"

for d in `seq 1 $maxdist`
do
  cat "$experimentDir"/est*/estimates_distance_"$d"_part_*| sort -gk1,1 > "$outdir"/dist_"$d"
  for size in `seq 1 $sampleSize`
  do
  	node=$RANDOM
  	node=$((node%maxNodeId))
    nn=`python "$thisDir"/../../main/algos/bitprop/supporters.py $graph $d $node`
    estimated=`awk -v node=$node '$1 == node {print $2; exit}' "$outdir"/dist_"$d"`
    if [ "$estimated" != "" ]; then
      echo $node $nn $estimated >> "$outdir"/diff_"$d"
    fi
  done
  awk -f "$thisDir"/are.awk "$outdir"/diff_"$d" > "$outdir"/rae_"$d"
done
