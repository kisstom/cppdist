#!/bin/bash -eu 

if [ $# != 4 ]; then
  echo "Usage numMessages numReceivers outputPrefix  msgSize" 
  exit
fi

numMessages="$1"
numReceivers="$2"
outputPrefix="$3"
msgSize="$4"
masterPort=7000
triggerPort=8000
bindir=../../../../bin/test/useFileSystem/common/socket/

echo "experiment " $numMessages
exportCommand="export LD_LIBRARY_PATH=~/libs/;"
outputDir="$outputPrefix"_"$numMessages"_"$msgSize"
mkdir -p "$outputDir"

"$bindir"/server_perform_test $masterPort \
    "$numMessages" $msgSize "$numReceivers" "$triggerPort" > "$outputDir"/log &

for i in `seq 0 $((numReceivers-1))`
do
  sleep 1
  ssh  hadoop0$((i%4+1)) "mkdir -p $outputDir"

  ssh hadoop0$((i%4+1)) "$exportCommand ./bin/client_perform_test $masterPort $numMessages $msgSize $triggerPort 1>$outputDir/client_log_$i 2>&1 &"
done

sleep 1

python ../../../main/common/zmq/zmq_start.py "$triggerPort"

