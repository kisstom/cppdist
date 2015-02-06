/*
 * split_by_row_job.cpp
 *
 *  Created on: 2013.12.11.
 *      Author: kisstom
 */

#include "split_by_row.h"
#include <string.h>

void usage() {
	fprintf(stderr, "Args: input_file log_file output_prefix num_nodes_per_part slavery_file\n");
}

int main (int argc, char* argv[]) {
	if (6 != argc) {
	  usage();
	  return 1;
	}

	string input = string(argv[1]);
	string logfile = string(argv[2]);
	char outputPrefix[1024];
	strcpy(outputPrefix, argv[3]);
	long numNodesPart;
	sscanf(argv[4], "%ld", &numNodesPart);
	string slaveryFile = string(argv[5]);

	SplitByRow splitByRow(input, logfile, outputPrefix, numNodesPart, slaveryFile);
	if (!splitByRow.init()) return 1;

	splitByRow.run();
	return 0;
}


