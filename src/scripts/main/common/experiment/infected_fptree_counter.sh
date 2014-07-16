#!/bin/bash -eu

thisDir=$(dirname $0)
outdir=$1

. $thisDir/../tools/create_experiment_info.sh

export LD_LIBRARY_PATH=/home/kisstom/git/Cppdist/src/dep/gmp/lib/:/home/kisstom/git/Cppdist/src/dep/log4cpp/lib/

"$thisDir"/../../../../bin/main/dmoz/infectedTreeComputerJob /mnt/idms/_bck/store/kisstom/data/portugal/reindexed.nopara.inv /mnt/idms/kisstom/run/distributed/portugal/simrank_odd_even/fptree_infection/simrank_on_top200M/concat.txt "$outdir"/output.txt 10 200000000 205000000 210000000 215000000 220000000 225000000 230000000 235000000 240000000 245000000
