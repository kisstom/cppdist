#!/bin/bash -eu

workdir=`mktemp`

thisDir=$(dirnam $0)

origPr=$1
newPr=$2
shuffleIndices=$3

awk -v shuffleIndexFile=$shuffleIndices -f $thisDir/get_orig_pr.awk $newPr > $workdir/testOrigPr
head -1000 $workdir/testOrigPr > $workdir/testOrigPr.h1000
head -1000 $origPr > $workdir/origPr.h1000
awk -v egyik=$workdir/origPr.h1000 -f $thisDir/../common/measures/rmse.awk $workdir/testOrigPr.h1000