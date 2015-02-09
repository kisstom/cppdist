# "Ez egy test komment"
thisDir=$(dirname $0)
SRCDIR=$thisDir/../../../

echo "Computes simrank and distance from tree and measures with kendal."
if [ $# -lt 1 ]; then
  echo "fp_with_pathindex outdir path_len const num_pathes"
  exit
fi

fp_with_pathindex=$1
outdir=$2

#simrank pmeters
path_len=$3
const=$4
num_pathes=$5

if [ $# -gt 5 ]; then
  node_topic=$6
  compute_topic_dist=1
else
  compute_topic_dist=0
fi

mkdir -p $outdir
echo "computing fp pathes"

#sort by node node then by fp inswx
sort -gk2,2 -gk1,1 $fp_with_pathindex| awk '{for (i=2; i < NF; ++i) printf("%s ", $i); printf "%s\n",$NF}' > $outdir/fingerprints.txt

echo "computing number of lines in output"
wcl=`wc -l $fp_with_pathindex|awk '{print $1}'`
nodes=$((wcl/num_pathes))

echo "determining the intersection pathes"
for x in `seq 2 $((path_len+1))`
do
  echo "pathindex $((x-1))"
  index=$((x+1))
  sort -gk1,1 -gk$index,$index $fp_with_pathindex > $outdir/tmp.sort
  awk -v field_num=$index -f $SRCDIR/scripts/main/simrank/get_section_new.awk $outdir/tmp.sort > $outdir/pairs_$index
done

echo "uniq the pairs"
$SRCDIR/scripts/main/simrank/uniq.sh $outdir $((path_len+1))

echo "number of pairs"
wc -l $outdir/concat.ord.sort.merge.u

echo "computing simrank"
echo $SRCDIR/bin/main/dmoz/get_simrank $nodes $path_len $num_pathes $const $outdir/fingerprints.txt $outdir/concat.ord.sort.merge.u $outdir/concat.ord.sort.merge.u.sr

$SRCDIR/bin/main/dmoz/get_simrank $nodes $path_len $num_pathes $const $outdir/fingerprints.txt $outdir/concat.ord.sort.merge.u $outdir/concat.ord.sort.merge.u.sr

awk -v input=$outdir/concat.ord.sort.merge.u.sr -f $SRCDIR/scripts/main/simrank/extend_pairs.awk $outdir/concat.ord.sort.merge.u.sr > $outdir/concat.ord.sort.merge.u.sr.ext
sort -grk1,1 -grk3,3  $outdir/concat.ord.sort.merge.u.sr.ext > $outdir/concat.ord.sort.merge.u.sr.ext.sort

if [ $compute_topic_dist == 1 ]; then
  
  $SRCDIR/bin/main/dmoz/distance_calculator  $outdir/concat.ord.sort.merge.u $node_topic $outdir/concat.ord.sort.merge.u.topic
  awk -v input=$outdir/concat.ord.sort.merge.u.topic -f $SRCDIR/scripts/main/simrank/extend_pairs.awk $outdir/concat.ord.sort.merge.u.topic > $outdir/concat.ord.sort.merge.u.topic.ext
  sort -grk1,1 -grk3,3  $outdir/concat.ord.sort.merge.u.topic.ext > $outdir/concat.ord.sort.merge.u.topic.ext.sort
  python $SRCDIR/scripts/main/algos/bitprop/kendal.py $outdir/concat.ord.sort.merge.u.sr.ext.sort  $outdir/concat.ord.sort.merge.u.topic.ext.sort 20 0.001

fi
