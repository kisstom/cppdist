#!/bin/bash -eu

thisDir=$(dirname $0)
outdir=$1

. $thisDir/../tools/create_expriment_info

"$thisDir"/../../../../bin/main/dmoz/infectedTreeComputerJob /mnt/idms/_bck/store/kisstom/data/portugal/reindexed.nopara.inv concat.txt "$outdir"/output.txt 10 200000000 205000000 210000000 215000000 220000000 225000000 230000000 235000000 240000000 245000000