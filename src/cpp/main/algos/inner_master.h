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

using std::string;

class Master;

class InnerMaster {
public:
	virtual bool nextIter() = 0;
	virtual void addInfoForNodes(char* ss);
	void setMaster(Master*);
	virtual void init() {}
	virtual ~InnerMaster() {}

private:
	Master* master_;
};


#endif /* INNER_MASTER_H_ */
