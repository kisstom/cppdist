/*
 * receiver_thread.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "receiver_thread.h"

/*ReceiverThread::ReceiverThread(Algo* algo) {
	algo_ = algo;
	logger_ = &log4cpp::Category::getInstance(std::string("ReceiverThread"));
}*/

ReceiverThread::ReceiverThread(AlgoBase* algoBase) {
  algoBase_ = algoBase;
  logger_ = &log4cpp::Category::getInstance(std::string("ReceiverThread"));
}

void ReceiverThread::main() {
	algoBase_->receiver();
}

