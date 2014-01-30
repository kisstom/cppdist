function printI() {
  if (printed == 0) {
    printf "%d", $i
    printed = 1
  } else {
    printf " %d", $i
  }
}

function thinOldEdges() {
  for (i = 1; i <= NF; ++i) {
    if ($i < max_node/2) {
      if (rand() < 0.2) {
        printI()
      }
    } else {
      printI()
    }
  }
}

function get_biased_neighb() {
  r = int(rand() * max_node / 4)
  #if (r >= max_node / 2) {
  #  r += max_node/2
  #}
  return r
}

function storeLine() {
  for (i = 1; i <= NF; ++i) neighbor_buffer[$i]
}

function denseOldEdges() {
  num_neighb = int(1.5 * max_neighbors)
  for (i = 1; i <= num_neighb; ++i) {
    edge = get_biased_neighb()
    if (edge in neighbor_buffer) continue

    if (printed == 0) {
      printf "%d", edge
      printed = 1
    } else {
      printf " %d", edge
    }

    neighbor_buffer[edge]
  }
  delete neighbor_buffer
}

function usage() {
  print "awk -v seed=13 -v max_neighbors -v max_node -f generate_biased_update.awk"
}

BEGIN{
  if (max_node==""|| max_neighbors == "") {
    usage()
    exit
  }

  if (seed == "") seed = 13
  srand(seed)
}
{
  printed = 0
  if (NR > max_node/4 && NR <= max_node /2 ) {
    thinOldEdges()
  } else if (NR > max_node/2 && NR <= max_node * 3/4) {
    printf $0
    storeLine()
    if (NF > 0) printed = 1
    denseOldEdges()
  } else {
    printf $0
  }
  printf "\n"
}

