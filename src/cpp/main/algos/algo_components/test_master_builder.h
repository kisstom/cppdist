/*
 * test_master_builder.h
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#ifndef TEST_MASTER_BUILDER_H_
#define TEST_MASTER_BUILDER_H_

#include "IMasterBuilder.h"

class TestMasterBuilder : public IMasterBuilder {
public:
	TestMasterBuilder();
	void setTestSlaveConfig(vector<Slave>* slaves);
protected:
	Master* createMaster(unordered_map<string, string>*);
private:
	vector<Slave>* slaves_;
	log4cpp::Category* logger_;
};


#endif /* TEST_MASTER_BUILDER_H_ */
