/*
 * simple_mock_inner_master.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef SIMPLE_MOCK_INNER_MASTER_H_
#define SIMPLE_MOCK_INNER_MASTER_H_

#include "../algo_components/inner_master.h"
#include "log4cpp/Category.hh"

class SimpleMockInnerMaster : public InnerMaster {
public:
	SimpleMockInnerMaster();
	bool nextIter();
	//void addInfoForNodes(char* ss);
private:
	int iteration_;
	log4cpp::Category* logger_;
};


#endif /* SIMPLE_MOCK_INNER_MASTER_H_ */
