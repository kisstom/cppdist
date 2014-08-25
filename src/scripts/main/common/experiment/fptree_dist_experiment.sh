#!/bin/bash -eu

if [ "$#" != 2 ]; then
	echo "args: outdir fptreefile"
	exit 1
fi

thisDir=$(dirname $0)

outdir="$1"
rm -rf "$outdir"
mkdir "$outdir"

. $thisDir/../tools/create_experiment_info.sh

fptreeFile="$2"

if [ ! -f "$fptreeFile" ]; then
	echo "Cannot reach $fptreeFile." 
	exit 1
fi

awk -f $thisDir/../../simrank/histogramms/tree_size.awk $fptreeFile > $outdir/fptree_deg_dist.txt

sort -gk1,1 $outdir/fptree_deg_dist.txt > $outdir/fptree_deg_dist.sort

awk -v st=0 -v buck=1000 -v xcol=1 -v ycol=2 -f $thisDir/../tools/hist.awk  $outdir/fptree_deg_dist.sort > $outdir/fptree_deg_hist_b10.txt
