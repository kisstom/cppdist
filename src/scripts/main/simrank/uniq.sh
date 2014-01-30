outdir=$1
max_index=$2

rm -f $outdir/concat
for x in `seq 2 $max_index`
do
  x=$((x+1))
  cat $outdir/pairs_$x >> $outdir/concat
done

awk '{if ($1 > $2) {print $2, $1} else {print $1, $2}}' $outdir/concat > $outdir/concat.ord

sort -gk1,1 -gk2,2 $outdir/concat.ord > $outdir/concat.ord.sort

awk '{print $1","$2}' $outdir/concat.ord.sort > $outdir/concat.ord.sort.merge

uniq $outdir/concat.ord.sort.merge | awk -v FS="," '{print $1, $2}' > $outdir/concat.ord.sort.merge.u


