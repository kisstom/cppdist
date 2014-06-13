/*
 * IMasterBuilder.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "IMasterBuilder.h"

Master* IMasterBuilder::buildFromConfig(unordered_map<string, string>* params) {
	string masterType = (*params)["MASTER_TYPE"];
	InnerMasterFactory innerMasterFactory;
	Master* master = NULL;
	master = createMaster(params);
	InnerMaster* innerMaster;

  innerMaster = innerMasterFactory.createInnerInnerMaster(params);
  master->setInnerMaster(innerMaster);
  innerMaster->setMaster(master);

	return master;
}


