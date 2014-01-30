a=`mktemp`
b=`mktemp`

tail -$3 $1 > $a
tail -$3 $2 > $b

awk -v egyik=$a -f rmse.awk $b
rm $a $b
