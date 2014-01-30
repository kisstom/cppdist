/*
 * remove_paralel_job.cpp
 *
 *  Created on: 2013.11.05.
 *      Author: kisstom
 */

#include "remove_paralel.h"

int main (int argc, char* argv[]) {
	int bufferLen;
	sscanf(argv[1], "%d", &bufferLen);
	RemoveParalel remover(bufferLen);
	remover.process(string(argv[2]), argv[3]);
}


