#!/bin/bash -eu

logDir=$1
findIterAwk="/home/kisstom/git/Cppdist/src/scripts/main/common/tools/find_iter_gap.awk"
printSenderAwk="/home/kisstom/git/Cppdist/src/scripts/main/common/tools/print_sender_time_milli.awk"
numPart=$2

cumulate=0.0
for ind in `seq 0 9`
do
  max=0 
  for x in `seq 0 $numPart`
  do
    time=`awk -v ind=$ind -f "$findIterAwk" "$logDir"/log_$x | awk -f "$printSenderAwk"`
    compare=`echo "$time>$max"|bc -l`
    if [ "$compare" == "1" ]; then
      max=$time
    fi
  done
  cumulate=`echo "$cumulate+$max"|bc -l`
#  echo $cumulate
done

echo "$cumulate/10"|bc -l
