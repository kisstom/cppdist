/*
 * test_pseudo_random.cpp
 *
 *  Created on: 2014.01.28.
 *      Author: kisstom
 */

#include "../../main/common/random/hash_pseudo_random.h"
#include <stdio.h>


int main (int argc, char* argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Usage: test_pseudo_random input_node_fp_path output seed.\n");
		return 1;
	}

	FILE* f = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

	if (NULL == f || NULL == output) {
		fprintf(stderr, "Error opening files %s %s.\n", argv[1], argv[2]);
		return 1;
	}

  int seed;
  sscanf(argv[3], "%d", &seed);

	long node;
	short fpIndex;
	short pathIndex;
	HashPseudoRandom random(seed);

	while (!feof(f)) {
		fscanf(f, "%ld %hd %hd\n", &node, &fpIndex, &pathIndex);
	  int r = random.get(fpIndex, pathIndex, node);
		fprintf(output, "%d\n", r);
	}

	fclose(f);
	fclose(output);
	return 0;
}

