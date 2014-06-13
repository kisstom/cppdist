/*
 * pagerank_master.h
 *
 *  Created on: 2014.05.09.
 *      Author: kisstom
 */

#ifndef PAGERANK_MASTER_H_
#define PAGERANK_MASTER_H_

#include "../inner_master.h"

class PagerankMaster : public InnerMaster {
public:
  bool nextIter();
  void addInfoForNodes(char* ss);
};


#endif /* PAGERANK_MASTER_H_ */
