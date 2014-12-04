#!/bin/bash -eu

rmse=`paste <(cat $1) <(cat $2)   |awk '{d = ($2 - $4); rmse += d*d}END{printf "%.10lf", sqrt(rmse/NR)}'`
avg1=`cat $1 | awk '{avg += $2}END{printf "%.10lf", avg/NR}'`
avg2=`cat $2 | awk '{avg += $2}END{printf "%.10lf", avg/NR}'`

echo "rmse avg1 avg2"
echo $rmse $avg1 $avg2

echo "rel err"
echo "$rmse/$avg1"|bc -l
echo "$rmse/$avg2"|bc -l
