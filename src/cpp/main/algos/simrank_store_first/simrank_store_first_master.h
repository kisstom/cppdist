/*
 * simrank_store_first_master.h
 *
 *  Created on: 2013.11.06.
 *      Author: kisstom
 */

#ifndef SIMRANK_STORE_FIRST_MASTER_H_
#define SIMRANK_STORE_FIRST_MASTER_H_

#include "../algo_components/inner_master.h"

class SimrankStoreFirstMaster : public InnerMaster {
	//bool nextIter();
  void addInfoForNodes(char* ss);
};


#endif /* SIMRANK_STORE_FIRST_MASTER_H_ */
