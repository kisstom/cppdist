#!/bin/bash -eu

if [ "$#" != 2 ]; then
	echo "args: outdir inputdata"
	exit 1
fi

thisDir=$(dirname $0)

outdir="$1"
data="$2"

rm -rf "$outdir"
mkdir "$outdir"

. $thisDir/../tools/create_experiment_info.sh

#inversePortugal=/mnt/idms/_bck/store/kisstom/data/portugal/reindexed.nopara.inv
#inversePortugal=/mnt/idms/_bck/store/kisstom/data/portugal/reindexed.nopara


if [ ! -f $data ]; then
	echo "Cannot reach $data." 
	exit 1
fi

awk -f $thisDir/../../simrank/histogramms/degree.awk $data > $outdir/degree_dist.txt

sort -gk1,1 $outdir/degree_dist.txt > $outdir/degree_dist.sort

awk -v st=0 -v buck=1000 -v xcol=1 -v ycol=2 -f $thisDir/../tools/hist.awk  $outdir/degree_dist.sort > $outdir/degree_hist_b1000.txt
