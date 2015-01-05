/*
 * simple_mock_algo.cpp
 *
 *  Created on: 2013.08.14.
 *      Author: kisstom
 */

#include "simple_mock_node.h"

SimpleMockNode::SimpleMockNode () {
	logger_ = &log4cpp::Category::getInstance(std::string("SimpleMockNode"));
	iteration_ = 0;
}

void SimpleMockNode::beforeIteration(string msg) {
  logger_->info("Before iteration %d", iteration_);
}

bool SimpleMockNode::afterIteration() {
	logger_->info("After iteration %d", iteration_);
	++iteration_;
	return true;
}

void SimpleMockNode::sender() {
	logger_->info("Starting sender %d.", partIndex_);
	int num_slaves = senderBuffer_->getBufferNum();
	long lmsg = 4;
	int imsg = 2;
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < num_slaves; ++i) {
			if (i == partIndex_) continue;
			senderBuffer_->setBreak(i);
			senderBuffer_->store(i, imsg);
			senderBuffer_->store(i, lmsg);
		}
	}
	algo_->sendAndSignal(partIndex_);
	logger_->info("Sender %d finished.", partIndex_);
}

void SimpleMockNode::update(int imsg, long lmsg) {
	//logger_->info("Update.");
}

void SimpleMockNode::initFromMaster(string) {

}

void SimpleMockNode::initData(string, long, long) {

}

void SimpleMockNode::final() {

}


