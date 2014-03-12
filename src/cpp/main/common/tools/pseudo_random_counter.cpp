/*
 * pseudo_random_counter.cpp
 *
 *  Created on: 2014.03.12.
 *      Author: kisstom
 */
#include "../random/hash_pseudo_random.h"
#include <stdio.h>

int main (int argc, char* argv[]) {
  FILE* input;
  if (argc == 1) {
    input = stdin;
  } else {
    input = fopen(argv[1], "r");
  }

  HashPseudoRandom random(13);
  long node;
  while (fscanf(input, "%ld", &node) != EOF) {
    printf("%d\n", random.get(0, 0, node));
  }

  if (argc != 1) {
    fclose(input);
  }
}


