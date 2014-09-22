#!/bin/bash -eu

trap 'rm -r $workdir' EXIT
workdir=`mktemp -d`

thisDir=$(readlink -f $(dirname $0))

resourceDir=$thisDir/../../../../../resources
defaultOrigPagerankIni=$resourceDir/integration/pagerank.ini
defaultPagerankIni=$resourceDir/integration/customPagerank.ini
customPagerankIni=$workdir/customPagerank.ini
origPagerankIni=$workdir/pagerank.ini
shuffleCustomPagerankIni=$workdir/shuffleCustomPagerank.ini

inputData=$resourceDir/filter_test/small.txt

configInputData=`echo $inputData|sed 's/\//\\\\\//g'`
binDir=`echo $thisDir/../../../../bin/|sed 's/\//\\\\\//g'`
scriptDir=`echo $thisDir/../../../../scripts/|sed 's/\//\\\\\//g'`
configWorkDir=`echo $workdir| sed 's/\//\\\\\//g'`
remoteDir=`echo $workdir/"partitions/"| sed 's/\//\\\\\//g'`
inversePartDir=`echo $workdir"/partitions/inversePart/"| sed 's/\//\\\\\//g'`
slaveryCfg=`echo $workdir/partitions/slavery.cfg| sed 's/\//\\\\\//g'`
shuffleData=`echo $workdir/shuffleDir/shuffle_reindex.clue|sed 's/\//\\\\\//g'`
outputDir=$workdir"/outdir/"


echo "Starting pagerank tester."
$thisDir/../../common/tools/converters/shuffle_reindexer.sh $inputData 2 $workdir/shuffleDir

pushd $thisDir/../
	cat $defaultPagerankIni|\
	sed 's/\(INPUT_DATA=\).*/\1'$configInputData'/'|\
	sed 's/\(BASE_DIR=\).*/\1'$configWorkDir'/'|\
	sed 's/\(REMOTE_DIR=\).*/\1'$remoteDir'/'|\
	sed 's/\(INVERSE_PARTITION_DIR=\).*/\1'$inversePartDir'/'|\
	sed 's/\(SLAVE_CONFIG=\).*/\1'$slaveryCfg'/'|\
	sed 's/\(BIN=\).*/\1'$binDir'/'|\
	sed 's/\(SCRIPTDIR=\).*/\1'$scriptDir'/' > $customPagerankIni

	fab cfg:$customPagerankIni computeAll
	cat $outputDir/out_* > $workdir/custom_concat

	cat $customPagerankIni|\
	sed 's/\(INPUT_DATA=\).*/\1'$shuffleData'/' > $shuffleCustomPagerankIni
	fab cfg:$shuffleCustomPagerankIni computeAll
	cat $outputDir/out_* > $workdir/custom_shuffle_concat

	cat $defaultOrigPagerankIni|\
	sed 's/\(INPUT_DATA=\).*/\1'$configInputData'/'|\
	sed 's/\(BASE_DIR=\).*/\1'$configWorkDir'/'|\
	sed 's/\(REMOTE_DIR=\).*/\1'$remoteDir'/'|\
	sed 's/\(BIN=\).*/\1'$binDir'/'|\
	sed 's/\(SCRIPTDIR=\).*/\1'$scriptDir'/' > $origPagerankIni	
	fab cfg:$origPagerankIni computeAll
	cat $outputDir/out_* > $workdir/orig_concat
popd

shuffleIndices=$workdir/shuffleDir/part_indices.sort

awk -v shuffleIndexFile=$shuffleIndices -f $thisDir/get_orig_pr.awk $workdir/custom_shuffle_concat \
> $workdir/testOrigPr

rmseDiff=`awk -v egyik=$workdir/testOrigPr -f $thisDir/../../common/measures/rmse.awk $workdir/custom_concat`
if [[ "$rmseDiff" != 0 ]]; then
	echo "custom pagerank differs from shuffle pagerank with rmse " $rmseDiff
	exit 1
fi

rmseDiff=`awk -v egyik=$workdir/orig_concat -f $thisDir/../../common/measures/rmse.awk $workdir/custom_concat`
if [[ "$rmseDiff" != 0 ]]; then
	echo "pagerank differs from custom with rmse " $rmseDiff
	exit 1
fi

echo "Pagerank tester finished."