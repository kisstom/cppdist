function get_biased_neighb (numrow) {
  if (numrow < max_node / 2.0) {
    r = rand()
    if (r < 0.9) {
      neighb = int(rand() * max_node / 2.0)
    } else {
      neighb = int(rand() * max_node / 2.0) + max_node / 2.0
    }
  } else {
    neighb = int(rand() * max_node)
  } 
  return neighb
}

function usage() {
  print "awk -v seed=13 -v swallow_prob -v max_neighbors -v max_node -f generate_biased.awk"
}

BEGIN{
  if (max_node==""|| max_neighbors == "" || swallow_prob == "") {
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
      printf "%d", get_biased_neighb(numrow)
    }
  
    for (i = 1; i < num_neighb; ++i) {
      edge = get_biased_neighb(numrow)
      if (!(edge in neighbor_buffer)) {
        printf " %d", edge
        neighbor_buffer[edge]
      }
    }

    print ""
    delete neighbor_buffer
  }

}
