# edge_upd_max
# edge_del_prob
# node_del_prob
# new_max_node
# swallow_prob

function usage () {
  print "usage: awk -v seed=13 -v edge_upd_max=10 -v edge_del_prob=0.01 -v node_del_prob=0.01 -v new_max_node=10 -v swallow_prob=0.01 -f update_matrix.awk input.txt"
}

BEGIN{
  if (edge_upd_max == "" || edge_del_prob==""|| node_del_prob="" || new_max_node==""|| swallow_prob=="") {
    usage()
    exit
  }

  if (seed == "") seed = 13
  srand(seed)
}
{
  if (rand() < node_del_prob) {
    print ""
    next
  }

  printed = 0
  for (i = 1; i <= NF; ++i) {
    if (rand() >= edge_del_prob) {
      if (printed == 0) {
        printf "%d", $i
        printed = 1;
      } else {
        printf " %d", $i
      }
    } 
  }

  num_neighb = int (rand() * edge_upd_max)
  for (j = 1; j <= num_neighb; ++j) {
    if (printed == 0) {
      printf "%d", int(rand() * new_max_node)
      printed = 1
    } else {
      printf " %d", int(rand() * new_max_node)
    }
  }

  print ""
}
END{
  for (numrow = 0; numrow < new_max_node - NR; ++numrow) {
    if (rand() < swallow_prob) {
      print ""
      continue
    }
    num_neighb = int (rand() * edge_upd_max)
    if (num_neighb > 0) {
      printf "%d", int(rand() * new_max_node)
    }
  
    for (i = 1; i < num_neighb; ++i) {
      printf " %d", int(rand() * new_max_node)
    }

    print ""

  }
}
