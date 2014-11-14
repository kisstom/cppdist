#!/bin/bash 

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

outputDir="$outputPrefix"_"$numMessages"_"$msgSize"
mkdir -p "$outputDir"

"$bindir"/server_perform_test $((masterPort)) \
    "$numMessages" $msgSize "$numReceivers" "$triggerPort" > "$outputDir"/log &

for i in `seq 0 $((numReceivers-1))`
do
  sleep 1

  "$bindir"/client_perform_test "$masterPort" \
     "$numMessages" "$msgSize" "$triggerPort"  > "$outputDir"/client_log_"$i"  &
done

sleep 1

python ../../../main/common/zmq/zmq_start.py "$triggerPort"

