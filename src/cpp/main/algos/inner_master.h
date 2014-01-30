/*
 * inner_master.h
 *
 *  Created on: 2013.08.13.
 *      Author: kisstom
 */

#ifndef INNER_MASTER_H_
#define INNER_MASTER_H_

#include <string>
using std::string;

class InnerMaster {
public:
	virtual bool nextIter() = 0;
	virtual void addInfoForNodes(string* ss) = 0;
	virtual ~InnerMaster() {}
};


#endif /* INNER_MASTER_H_ */
