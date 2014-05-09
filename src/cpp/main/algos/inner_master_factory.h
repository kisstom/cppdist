/*
 * inner_master_builder.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef INNER_MASTER_FACTORY_H_
#define INNER_MASTER_FACTORY_H_

#include "../common/util/cfg_reader.h"
#include "pagerank/pagerank_master.h"
#include "simrank_store_first/simrank_store_first_master.h"
#include "simrank_odd_even/simrank_odd_even_master.h"
#include "inner_master.h"
#include <log4cpp/Category.hh>

class InnerMasterFactory {
public:
	InnerMasterFactory();
	InnerMaster* createInnerInnerMaster(unordered_map<string, string>*);
private:
	log4cpp::Category* logger_;
};



#endif /* INNER_MASTER_FACTORY_H_ */
