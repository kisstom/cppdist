
for d in `seq 1 4`
do
  rm -rf tmp_$d 
  for x in `seq 0 999`
  do
    a=`python supporters.py /home/kisstom/test/Clue2012/connection/rand_col20_row1k $d $x`
    echo $a >> tmp_$d 
  done
  wc -l tmp_$d 
done
