/*
 * IMasterBuilder.h
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#ifndef MASTER_BUILDER_H_
#define MASTER_BUILDER_H_

#include "master_base/master_base.h"
#include "inner_master_factory.h"
#include "factories/imaster_factory.h"
#include "log4cpp/Category.hh"

using std::string;

class MasterBuilder {
public:
  MasterBuilder();
	MasterBase* buildFromConfig(unordered_map<string, string>* params);
  //void setMasterFactory(IMasterFactory*);
  ~MasterBuilder() {}
private:
  log4cpp::Category* logger_;
  IMasterFactory* masterFactory;
};

#endif /* MASTERBUILDER_H_ */
