num=112; crawls=$(for x in `seq 0 $((num-1))`; do echo -ne " "$((1250*x+200000)); done); ../bin/main/dmoz/infectedTreeComputerJob ~/data/webgraph/notredam/converted/reindexed_graph.clue.inv /home/kisstom/test/distributed/simrank_odd_even/notredam_fptree_inf/concat.txt /home/kisstom/test/distributed/simrank_odd_even/notredam_fptree_inf/infection $num $crawls
