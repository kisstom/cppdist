#!/bin/bash -eu

if [ "$#" != 4 ]; then
 echo params: resultDir numPathes topK trustedNodes
 exit 1
fi

thisDir=$(dirname $0)
SRCDIR=$thisDir/../../../../

export LD_LIBRARY_PATH=$SRCDIR/dep/log4cpp/lib/
resultDir=$1
num=`ls "$resultDir"|grep crawl|wc -l` 

numPathes=$2
topK=$3
trustedNodes=$4

ndcgOut="$resultDir"/ndcgOut
rmseOut="$resultDir"/rmseOut

awk -f $SRCDIR/scripts/main/common/tools/itemsRandomScore.awk $trustedNodes > "$resultDir"/randomScore

for x in `seq 0 $((num-1))`
do

  mkdir -p "$resultDir"/crawl_"$x"/experiment

  cat "$resultDir"/crawl_"$x"/out_* > "$resultDir"/crawl_"$x"/experiment/out

  $SRCDIR/scripts/main/simrank/compute_simrank_from_fp.sh \
  "$resultDir"/crawl_"$x"/experiment/out  "$resultDir"/crawl_"$x"/experiment/simrank 10 0.9 $numPathes

  python $SRCDIR/scripts/main/common/measures/ndcg.py  \
  "$resultDir"/crawl_0/experiment/simrank/concat.ord.sort.merge.u.sr.ext.sort \
  "$resultDir"/crawl_"$x"/experiment/simrank/concat.ord.sort.merge.u.sr.ext.sort $topK >> $ndcgOut

  awk -v egyik="$resultDir"/crawl_0/experiment/simrank/concat.ord.sort.merge.u.sr.ext.sort -f \
  $SRCDIR/scripts/main/common/measures/rmseForPairs.awk \
  "$resultDir"/crawl_"$x"/experiment/simrank/concat.ord.sort.merge.u.sr.ext.sort >> $rmseOut
done

#echo random score >> $ndcgOut

python $SRCDIR/scripts/main/common/measures/ndcg.py  \
"$resultDir"/crawl_0/experiment/simrank/concat.ord.sort.merge.u.sr.ext.sort \
"$resultDir"/randomScore $topK >> $ndcgOut

