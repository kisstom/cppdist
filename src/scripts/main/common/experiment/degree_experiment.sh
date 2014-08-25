#!/bin/bash -eu

if [ "$#" != 1 ]; then
	echo "args: outdir" 
fi

thisDir=$(dirname $0)

outdir="$1"
rm -rf "$outdir"
mkdir "$outdir"

. $thisDir/../tools/create_experiment_info.sh

inversePortugal=/mnt/idms/_bck/store/kisstom/data/portugal/reindexed.nopara.inv

if [ ! -f $inversePortugal ]; then
	echo "Cannot reach $inversePortugal." 
	exit 1
fi

awk -f $thisDir/../../simrank/histogramms/degree.awk $inversePortugal > $outdir/degree_dist.txt

sort -gk2,2 $outdir/degree_dist.txt > $outdir/degree_dist.sort

awk -v st=0 -v buck=10 -v col=2 -f $thisDir/../tools/hist.awk  $outdir/degree_dist.sort > $outdir/degree_hist_b10.txt
