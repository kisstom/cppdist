#!/bin/bash -eu

logDir=$1
findIterAwk="/home/kisstom/git/Cppdist/src/scripts/main/common/tools/find_iter_gap.awk"
printSenderAwk="/home/kisstom/git/Cppdist/src/scripts/main/common/tools/print_sender_time_milli.awk"

for ind in `seq 0 9`
do 
  for x in `seq 0 19`
  do
    awk -v ind=$ind -f "$findIterAwk" "$logDir"/log_$x | awk -f "$printSenderAwk"
  done
  echo "----------------------------------"
done
