# args:
# swallow_prob
# max_neighbors
# max_node

function usage() {
  print "awk -v seed=13 -v swallow_prob -v max_neighbors -v max_node -f gen_matrix.awk"
}

BEGIN{
  if (swallow_prob == "" || max_neighbors == "" || max_node == "") {
    usage()
    exit
  }

  if (seed == "") seed = 13
  srand(seed)

  for (numrow = 0; numrow < max_node; ++numrow) {
    if (rand() < swallow_prob) {
      print ""
      continue
    }
    num_neighb = int (rand() * max_neighbors)
    if (num_neighb > 0) {
      printf "%d", int(rand() * max_node)
    }
  
    for (i = 1; i < num_neighb; ++i) {
      printf " %d", int(rand() * max_node)
    }

    print ""
  }
}
