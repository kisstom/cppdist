/*
 * inner_master_builder.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "inner_master_factory.h"


InnerMasterFactory::InnerMasterFactory() {
	logger_ = &log4cpp::Category::getInstance(std::string("InnerMasterFactory"));
}

InnerMaster* InnerMasterFactory::createInnerInnerMaster(unordered_map<string, string>* params) {
	string innerMasterType = (*params)["INNER_MASTER_TYPE"];
	InnerMaster* innerMaster = NULL;

	if (innerMasterType.compare("SIMRANK_UPDATE") == 0) {
	  //innerMaster = new SimrankUpdateMaster;
		innerMaster = NULL;
	} else if (innerMasterType.compare("SIMRANK_STORE_FIRST") == 0) {
		innerMaster = new SimrankStoreFirstMaster;
	} else if (innerMasterType.compare("SIMRANK_ODD_EVEN") == 0) {
		innerMaster = new SimrankOddEvenMaster;
	} else {
		logger_->error("Unknown inner master type %s", innerMasterType.c_str());
	}

	return innerMaster;
}

