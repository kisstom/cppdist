/*
 * mock_inner_master.h
 *
 *  Created on: 2013.08.14.
 *      Author: kisstom
 */

#ifndef MOCK_INNER_MASTER_H_
#define MOCK_INNER_MASTER_H_

#include "gmock/gmock.h"


using std::stringstream;

class MockInnerMaster : public InnerMaster {
public:
	MOCK_METHOD0(nextIter, bool());
	MOCK_METHOD1(addInfoForNodes, void(string* ss));
};


#endif /* MOCK_INNER_MASTER_H_ */
