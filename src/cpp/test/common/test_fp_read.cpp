/*
 * test_fp_read.cpp
 *
 *  Created on: 2013.08.23.
 *      Author: kisstom
 */

#include <list>
#include "../../main/common/components/fingerprint_reader.h"

using std::list;

int main () {
	FingerprintReader fpReader;
	list<RandomWalk>* rws = new list<RandomWalk>;
	fpReader.setRWContainer(rws);
	char fname[1024] = "/home/kisstom/test/distributed/fingerprint/fp_with_path_indexes";
  fpReader.readFile(fname ,0, 100);

}

