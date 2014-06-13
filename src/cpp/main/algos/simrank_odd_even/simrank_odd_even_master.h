/*
 * simrank_odd_even_master.h
 *
 *  Created on: 2014.01.14.
 *      Author: kisstom
 */

#ifndef SIMRANK_ODD_EVEN_MASTER_H_
#define SIMRANK_ODD_EVEN_MASTER_H_


#include "../inner_master.h"

class SimrankOddEvenMaster : public InnerMaster {
	bool nextIter();
  void addInfoForNodes(char* ss);
};

#endif /* SIMRANK_ODD_EVEN_MASTER_H_ */
