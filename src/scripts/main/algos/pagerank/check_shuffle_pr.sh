#!/bin/bash -eu

#trap 'ls $workdir' EXIT
workdir=`mktemp -d`

thisDir=$(readlink -f $(dirname $0))

echo $thisDir

resourceDir=$thisDir/../../../../../resources
defaultPagerankIni=$resourceDir/integration/customPagerank.ini
customPagerankIni=$workdir/customPagerank.ini
shuffleCustomPagerankIni=$workdir/shuffleCustomPagerank.ini
inputData=$resourceDir/filter_test/small.txt
binDir=$thisDir/../../../../../bin/
scriptDir=$thisDir/../../../../../scripts/


localDir=$workdir/"outdir"
remoteDir=$workdir/"partitions"
inversePartDir=$remoteDir/"inversePart"
slaveryCfg=$remoteDir/"slavery.cfg"

$thisDir/../../common/tools/converters/shuffle_reindexer.sh $inputData 2 $workdir/shuffleDir
shuffleData=$workdir/shuffleDir/shuffle_reindex.clue

pushd $thisDir/../

	echo cat $defaultPagerankIni|\
	sed 's/\(INPUT_DATA=\).*/\1'$inputData'/'|\
	sed 's/\(BASE_DIR=\).*/\1'$workdir'/'|\
	sed 's/\(REMOTE_DIR=\).*/\1'$remoteDir'/'|\
	sed 's/\(INVERSE_PARTITION_DIR=\).*/\1'$inversePartDir'/'|\
	sed 's/\(SLAVE_CONFIG=\).*/\1'$slaveryCfg'/'|\
	sed 's/\(BIN=\).*/\1'$binDir'/'|\
	sed 's/\(SCRIPTDIR=\).*/\1'$scriptDir'/' > $customPagerankIni

	exit
	fab cfg:$customPagerankIni computeAll
	cat $outputDir/out_* > $workdir/custom_concat

	cat $customPagerankIni|\
	sed 's/\(INPUT_DATA=\).*/\1'$shuffleData'/' > $shuffleCustomPagerankIni

	fab cfg:$shuffleCustomPagerankIni computeAll

	cat $outputDir/out_* > $workdir/custom_shuffle_concat
popd

shuffleIndices=$workdir/shuffleDir/shuffle_reindex.sort

awk -v shuffleIndexFile=$shuffleIndices -f $thisDir/get_orig_pr.awk $workdir/custom_shuffle_concat \
> $workdir/testOrigPr

awk -v egyik=$workdir/testOrigPr -f $thisDir/../common/measures/rmse.awk $workdir/custom_concat