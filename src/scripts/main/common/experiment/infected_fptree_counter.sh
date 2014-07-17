#!/bin/bash -eu

thisDir=$(dirname $0)

if [ $# != 5 ]; then
  echo "params: outdir cfg numCrawls startCrawl nodeInCrawl"
  exit 1
fi

outdir=$1
cfg=$2
num=$3
start=$4
gap=$5
crawls=$(for x in `seq 0 $((num-1))`; do echo -ne " "$((gap*x+start)); done)

. $thisDir/../tools/create_experiment_info.sh

export LD_LIBRARY_PATH=/home/kisstom/git/Cppdist/src/dep/gmp/lib/:/home/kisstom/git/Cppdist/src/dep/log4cpp/lib/

pythonbrew venv use fabos

fab cfg:$cfg

exit 0

fab cfg:$cfg compute

# fab cfg default output
cat "$outdir"/outdir/out* > "$outdir"/concat.txt

"$thisDir"/../../../../bin/main/dmoz/infectedTreeComputerJob /mnt/idms/_bck/store/kisstom/data/portugal/reindexed.nopara.inv \
"$outdir"/concat.txt "$outdir"/output.txt $num $crawls
