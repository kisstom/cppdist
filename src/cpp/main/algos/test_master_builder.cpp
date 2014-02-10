/*
 * test_master_builder.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "test_master_builder.h"

TestMasterBuilder::TestMasterBuilder() {
	logger_ = &log4cpp::Category::getInstance(std::string("TestMasterBuilder"));
	slaves_ = NULL;
}

Master* TestMasterBuilder::createMaster(unordered_map<string, string>* params) {
	int master_port, num_slaves;

	sscanf((*params)["MASTER_PORT"].c_str(), "%d", &master_port);
	sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &num_slaves);

	Master* master = new Master(master_port, slaves_);
	return master;
}

void TestMasterBuilder::setTestSlaveConfig(vector<Slave>* slaves) {
	slaves_ = slaves;
}
