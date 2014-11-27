#!/bin/bash -eu

logDir=$1
numPart=$2

baseDir=$(readlink -f `dirname $0`)
findIterAwk="$baseDir/find_iter_gap.awk"
printSenderAwk="$baseDir/print_sender_time_milli.awk"

cumulate=0.0
for ind in `seq 0 9`
do
  max=0 
  for x in `seq 0 $numPart`
  do
    # Finds the starting and ending time of the ind. iteration for the 
    # x. node, and counts the difference.
    time=`awk -v ind=$ind -f "$findIterAwk" "$logDir"/log_$x | awk -f "$printSenderAwk"`
    compare=`echo "$time>$max"|bc -l`
    if [ "$compare" == "1" ]; then
      max=$time
    fi
  done
  # Sums the maximum of each iteration.
  cumulate=`echo "$cumulate+$max"|bc -l`
done

echo "$cumulate/10"|bc -l
