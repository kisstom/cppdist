/*
 * simank_update_master.h
 *
 *  Created on: 2013.08.13.
 *      Author: kisstom
 */

#ifndef SIMANK_UPDATE_MASTER_H_
#define SIMANK_UPDATE_MASTER_H_

#include "../inner_master.h"

class SimrankUpdateMaster : public InnerMaster {
public:
  //bool nextIter();
  void addInfoForNodes(char* ss);
};


#endif /* SIMANK_UPDATE_MASTER_H_ */
