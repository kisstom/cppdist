/*
 * ndcg_counter.cpp
 *
 *  Created on: 2013.07.23.
 *      Author: kisstom
 */

#include <stdio.h>
#include <string.h>
#include "ndcg.h"

void usage() {
	printf("Failed to start program. Usage: pathToBin/ndcg_counter scoreFile1 scoreFile2 topk\n");
}

int main(int argc, char* argv[]) {
	int topk;
	if (argc != 4) {
		usage();
		return 1;
	}

	if (EOF == sscanf(argv[3], "%d", &topk)) {
		printf("Topk not parsed.\n");
		usage();
		return 1;
	}

	char file1[1024];
	strcpy(file1, argv[1]);
	char file2[1024];
	strcpy(file2, argv[2]);

	NDCG ndcg(std::string(file1), std::string(file2), topk);
	ndcg.run();
}

