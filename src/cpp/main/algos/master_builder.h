/*
 * master_builder.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef MASTER_BUILDER_H_
#define MASTER_BUILDER_H_

#include "../common/util/cfg_reader.h"
#include "master.h"
#include "IMasterBuilder.h"
#include <log4cpp/Category.hh>
#include <vector>


class MasterBuilder : public IMasterBuilder {
public:
	MasterBuilder();
	Master* buildFromConfig(unordered_map<string, string>* params);
private:
	Master* createMaster(unordered_map<string, string>*);
	vector<Slave>* readSlaveConfigs(char* cfg, int num_slaves);
	log4cpp::Category* logger_;
};


#endif /* MASTER_BUILDER_H_ */
