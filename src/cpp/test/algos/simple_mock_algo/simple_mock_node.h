/*
 * simple_mock_node.h
 *
 *  Created on: 2013.08.14.
 *      Author: kisstom
 */

#ifndef SIMPLE_MOCK_NODE_H_
#define SIMPLE_MOCK_NODE_H_

#include "../../../main/algos/node.h"
#include "../../../main/common/components/sender_buffer.h"
#include "../../../main/algos/algo.h"
#include "log4cpp/Category.hh"

class SimpleMockNode : public Node {
public:
	SimpleMockNode();
	void beforeIteration(string msg);
	bool afterIteration();
	void sender();
	void initFromMaster(string);
	void initData(string, long, long);
	void final();
	void update(int, long);
	~SimpleMockNode() {}
private:
	int iteration_;

	//int partIndex_;
	//SenderBuffer* senderBuffer_;
	//Algo* algo_;
	log4cpp::Category* logger_;
};


#endif /* SIMPLE_MOCK_NODE_H_ */
