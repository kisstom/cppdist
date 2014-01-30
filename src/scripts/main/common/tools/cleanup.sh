
CONF=$1
. $CONF
. $SCRIPTDIR/main/algos/pagerank/delete_result.sh

OUTFILE=$2

if [ "$SCRIPTDIR" == "" ]; then
  echo "Valoszinu hiba, a script dir nincs definialva."
  exit
fi

python $SCRIPTDIR/main/common/concat.py $CONF $OUTFILE
delete_res $CONF
