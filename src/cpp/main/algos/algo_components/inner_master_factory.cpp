/*
 * inner_master_builder.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "inner_master_factory.h"
#include "../custom_non_block/custom_non_block_master.h"
#include "../custom_multi_nonblock/custom_multi_non_block_master.h"
#include "../pagerank_non_block/pagerank_non_block_master.h"
#include "../counter_inverse/counter_inverse_master.h"
#include "../counter_inverse_pagerank/counter_inverse_pagerank_master.h"
#include "../als/als_master.h"

InnerMasterFactory::InnerMasterFactory() {
	logger_ = &log4cpp::Category::getInstance(std::string("InnerMasterFactory"));
}

InnerMaster* InnerMasterFactory::createInnerInnerMaster(unordered_map<string, string>* params) {
	string innerMasterType = (*params)["INNER_MASTER_TYPE"];
	InnerMaster* innerMaster = NULL;

	if (innerMasterType.compare("SIMRANK_UPDATE") == 0) {
		innerMaster = NULL;
	} else if (innerMasterType.compare("SIMRANK_STORE_FIRST") == 0) {
		innerMaster = new SimrankStoreFirstMaster;
	} else if (innerMasterType.compare("SIMRANK_ODD_EVEN") == 0) {
		innerMaster = new SimrankOddEvenMaster;
	} else if (innerMasterType.compare("PAGERANK") == 0) {
	  innerMaster = new PagerankMaster;
	} else if (innerMasterType.compare("PAGERANK_NON_BLOCK") == 0) {
    innerMaster = new PagerankNonBlockMaster;
  } else if (innerMasterType.compare("PSIMRANK") == 0) {
	  innerMaster = createPSimrankMaster(params);
  } else if (innerMasterType.compare("SIMPLE_MOCK") == 0) {
    innerMaster = new SimpleMockInnerMaster;
  } else if (innerMasterType.compare("BITPROP") == 0) {
    innerMaster = createBitpropMaster(params);
  } else if (innerMasterType.compare("CLEVER_PAGERANK") == 0) {
    innerMaster = new CleverPagerankMaster;
  } else if (innerMasterType.compare("CUSTOM_NON_BLOCK") == 0) {
    innerMaster = new CustomNonBlockMaster;
  } else if (innerMasterType.compare("COUNTER_INVERSE") == 0) {
    innerMaster = new CounterInverseMaster;
  } else if (innerMasterType.compare("COUNTER_INVERSE_PAGERANK") == 0) {
    innerMaster = new CounterInversePagerankMaster;
  } else if (innerMasterType.compare("CUSTOM_MULTI_NONBLOCK") == 0) {
    innerMaster = new CustomMultiNonBlockMaster;
  } else if (innerMasterType.compare("ALS") == 0) {
    innerMaster = new AlsMaster;
  } else {
		logger_->error("Unknown inner master type %s", innerMasterType.c_str());
	}

	return innerMaster;
}

InnerMaster* InnerMasterFactory::createBitpropMaster(unordered_map<string, string>* params) {
  util_.checkParam(params, 1, "NEIGHBORHOOD_SIZE");
  short neighborhoodSize;
  sscanf((*params)["NEIGHBORHOOD_SIZE"].c_str(), "%hd", &neighborhoodSize);
  return new BitpropMaster(neighborhoodSize);
}

InnerMaster* InnerMasterFactory::createPSimrankMaster(unordered_map<string, string>* params) {
  PSimrankRandomGeneratorIFace* randomGen = NULL;
  if (params->find(string("RANDOM_TYPE")) == params->end()) {
    randomGen = getPSimrankRandom(params);
  } else {
    if ((*params)["RANDOM_TYPE"].compare("PSEUDO") == 0) {
      randomGen = getPSimrankPseudoRandom(params);
    } else if ((*params)["RANDOM_TYPE"].compare("SRAND") == 0) {
      randomGen = getPSimrankRandom(params);
    } else {
      logger_->error("Error unknoyn tpye of random generator %s",
          (*params)["RANDOM_TYPE"].c_str());
      throw ParamMissException("RANDOM_TYPE is missing.");
    }
  }

  long numNodes = -1;
  sscanf((*params)["NUMLINE"].c_str(), "%ld", &numNodes);
  PSimrankMaster* innerMaster = new PSimrankMaster(numNodes);
  innerMaster->setRandomGenerator(randomGen);

  return innerMaster;
}

PSimrankRandomGeneratorIFace* InnerMasterFactory::getPSimrankPseudoRandom(unordered_map<string, string>* params) {
  util_.checkParam(params, 2, "RANDOM_A", "RANDOM_B");

  string randomA = (*params)["RANDOM_A"];
  string randomB = (*params)["RANDOM_B"];


  vector<string> ra = util_.split(randomA, ',');
  vector<long> ral = util_.convertToLong(ra);

  vector<string> rb = util_.split(randomB, ',');
  vector<long> rbl = util_.convertToLong(rb);

  PSimrankRandomGeneratorIFace* rv = new PSimrankPseudoRandomGenerator(ral, rbl);
  return rv;
}

PSimrankRandomGeneratorIFace* InnerMasterFactory::getPSimrankRandom(unordered_map<string, string>* params) {
  util_.checkParam(params, 1, "SEED");

  long seed;
  if (params->find(string("SEED")) == params->end()) {
    seed = 13;
  } else {
    sscanf((*params)["SEED"].c_str(), "%ld", &seed);
  }
  return new PSimrankRandomGenerator(seed);
}

