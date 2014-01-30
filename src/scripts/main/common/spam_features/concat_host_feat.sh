
new=allfeat_joined
prev=prev_joined

cp pagerank.concat.less $prev
FEAT_NAME=PAGERANK

echo "HOSTNAME"
echo "$FEAT_NAME"_PRMEAN
echo "$FEAT_NAME"_PRDEV
echo "$FEAT_NAME"_MAXPRFEAT
echo "$FEAT_NAME"_HOSTFEAT

for algo in `ls *.concat`
do
  FEAT_NAME=`echo $algo| sed 's/\.[a-z]*//'| tr 'a-z' 'A-Z'`
  echo "$FEAT_NAME"_MEAN
  echo "$FEAT_NAME"_DEV
  echo "$FEAT_NAME"_MAXPRFEAT
  echo "$FEAT_NAME"_HOSTFEAT

  awk -v join_with_this_file=$algo -f /mnt/nessi4/home/kisstom/git/DistributedComp/DistributedFrame/src/scripts/main/common/concat_host_feat.awk $prev > $new
  mv $new $prev
done
