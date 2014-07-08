INPUT=$1
OUTPUTDIR=$2
thisDir=$(dirname $0)
SRCDIR=$thisDir/../../../../

#export LD_LIBRARY_PATH=/mnt/nessi4/home/kisstom/mylibs/

$SRCDIR/bin/main/common/graph_converter/continue_indexing $INPUT $OUTPUTDIR/reindex_hash.txt
echo "graph reindex map fertig"
date

numnodes=`wc -l $OUTPUTDIR/reindex_hash.txt|cut -f1 -d' '`
echo "number of nodes: " $numnodes

$SRCDIR/bin/main/common/graph_converter/reindexer $INPUT $OUTPUTDIR/reindex_hash.txt $OUTPUTDIR/reindexed_graph
echo "reindex done"
date

orig_numline=`wc -l $1| cut -f1 -d' '`
converted_numline=`wc -l $OUTPUTDIR/reindexed_graph| cut -f1 -d' '`

if [ $orig_numline != $converted_numline ]; then
  echo "Number of orig line " $orig_numline " is different from converted " $converted_numline 
fi
  
sort -T /home/kisstom/ -gk1,1 $OUTPUTDIR/reindexed_graph > $OUTPUTDIR/reindexed_graph.sort
echo "sort done"
date

$SRCDIR/bin/main/common/graph_converter/print_clue_format $OUTPUTDIR/reindexed_graph.sort $OUTPUTDIR/reindexed_graph.clue $numnodes
echo "clue format created"
date
