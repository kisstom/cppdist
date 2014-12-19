/*
 * master_builder.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "master_builder.h"


MasterBuilder::MasterBuilder() {
  logger_ = &log4cpp::Category::getInstance(std::string("MasterBuilder"));
}

MasterBase* MasterBuilder::buildFromConfig(unordered_map<string, string>* params) {
	string masterType = (*params)["MASTER_TYPE"];
	InnerMasterFactory innerMasterFactory;
	MasterBase* master = NULL;

	master = masterFactory->createMaster(params);
	InnerMaster* innerMaster = NULL;

  innerMaster = innerMasterFactory.createInnerInnerMaster(params);
  master->setInnerMaster(innerMaster);
  innerMaster->setMaster(master);
  innerMaster->init();

	return master;
}
