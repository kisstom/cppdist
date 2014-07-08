/*
 * inner_master_builder.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef INNER_MASTER_FACTORY_H_
#define INNER_MASTER_FACTORY_H_

#include "../common/util/cfg_reader.h"
#include "../common/util/param_miss_exception.h"
#include "../common/util/util.h"
#include "pagerank/pagerank_master.h"
#include "simrank_store_first/simrank_store_first_master.h"
#include "simrank_odd_even/simrank_odd_even_master.h"
#include "psimrank/psimrank_master.h"
#include "psimrank/psimrank_pseudo_random_generator.h"
#include "psimrank/psimrank_random_generator.h"
#include "inner_master.h"
#include "../../test/algos/simple_mock_algo/simple_mock_inner_master.h"
#include <log4cpp/Category.hh>

class InnerMasterFactory {
public:
	InnerMasterFactory();
	InnerMaster* createInnerInnerMaster(unordered_map<string, string>*);
private:
	InnerMaster* createPSimrankMaster(unordered_map<string, string>* params);
	PSimrankRandomGeneratorIFace* getPSimrankPseudoRandom(unordered_map<string, string>* params);
	PSimrankRandomGeneratorIFace* getPSimrankRandom(unordered_map<string, string>* params);
	Util util_;
	log4cpp::Category* logger_;
};



#endif /* INNER_MASTER_FACTORY_H_ */
