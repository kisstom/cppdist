/*
 * inner_master.h
 *
 *  Created on: 2013.08.13.
 *      Author: kisstom
 */

#ifndef INNER_MASTER_H_
#define INNER_MASTER_H_

#include <string>
#include "master.h"
#include "log4cpp/Category.hh"

using std::string;

class Master;

class InnerMaster {
public:
  InnerMaster();
	virtual bool nextIter();
	virtual void addInfoForNodes(char* ss);
	virtual void setMaster(Master*);
	virtual void init() {}
	virtual ~InnerMaster() {}

protected:
	log4cpp::Category* logger_;
	Master* master_;
};


#endif /* INNER_MASTER_H_ */
