function rand_nb() {
  return int (rand() * numrows)
}

function gen_uniq_row(nb, gens) {
  while (length(gens) < nb) {
    gens[rand_nb()]
  }
}


BEGIN{
  srand(13)

  # mnb
  # numrows
  for (i = 0; i < numrows; ++i) {
    nb = int (rand() * mnb)
    
    if (nb == 0) {
      printf("\n");
      continue;
    }

    delete gens    
    gens[rand_nb()]
    gen_uniq_row(nb, gens)

    first = 1
    for (x in gens) {
      if (first == 1) {
        printf("%d", x)
        first = 0
      } else {
        printf(" %d", x)
      }
    }
    printf("\n");
  }  
}
