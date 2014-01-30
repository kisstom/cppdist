#!/bin/bash 

. $1

NUMJOBS=0
while read line
do
  name=$(echo "$line"| sed 's/=\([0-9]\|[a-zA-Z]\|.\|_\|\/\)*//')
  var=$(echo "$line"|sed 's/\([a-zA-Z]\|_\)*=//')
  if [ "$name" == 'MACHINE' ]; then
    job=$(echo $var| sed 's/\([0-9]\|[a-zA-Z]\)*=//')
    NUMJOBS=$((NUMJOBS+job))
  fi
done < $1

rm -f $REMOTE_DIR/slavery* 
mkdir -p $REMOTE_DIR 

echo "Old files removed."

# Number of edges per partition.
echo $NUMEDGE, $NUMJOBS
foo=$((NUMEDGE%NUMJOBS))
echo "foo", $foo
if [ "$foo" == 0 ]; then
  NUMEDGE_PERPART=$((NUMEDGE/$NUMJOBS))
else
  NUMEDGE_PERPART=$((NUMEDGE/$NUMJOBS + 1))
fi
echo -e "Minimum number of edges in each part: $NUMEDGE_PERPART"

zipped_if_null=`echo $INPUT_DATA|sed 's/.*gz//'`

export LD_LIBRARY_PATH=/home/kisstom/mylibs/gflags/
if [ "$zipped_if_null" == "" ]; then
  echo "zipped_if_null"
  zcat $INPUT_DATA| $BIN/main/common/split_clueweb_vector_node -init_slave_port $INIT_SLAVE_PORT -numedge_perpart $NUMEDGE_PERPART -part_prefix $REMOTE_DIR/slavery  -partition_cfg $REMOTE_DIR/slavery.cfg 
else
  echo "cat"
  $BIN/main/common/split_clueweb_vector_node -input $INPUT_DATA -init_slave_port $INIT_SLAVE_PORT -numedge_perpart $NUMEDGE_PERPART -part_prefix $REMOTE_DIR/slavery  -partition_cfg $REMOTE_DIR/slavery.cfg 
fi
