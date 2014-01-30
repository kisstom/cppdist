check_run () {
  job=$1
  master_job=$2
  time=$3

  if [ "$time" == "" ]; then
    time=2
  fi

  while [ 1 == 1 ];
  do
    sum=$(ps x| grep -v grep | grep $master_job|wc -l)
    for((i=0;i<${#machines[@]};++i))
    do
      if [ ${machines[i]} == $MASTER_HOST ]; then
        runs=$(ssh ${machines[i]}  "ps x| grep -v grep | grep $job|wc -l")
        sum=$((sum+runs))
      else
        runs=$(ps x| grep -v grep | grep $job|wc -l)
        sum=$((sum+runs))
      fi
    done
    if [ $sum -gt 0 ]; then
      sleep $time
    else
      break
    fi
  done
  echo "Job is done."
}
