/*
 * sender_thread.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "sender_thread.h"

SenderThread::SenderThread(Node* node) {
	node_ = node;
	logger_ = &log4cpp::Category::getInstance(std::string("SenderThread"));
}

void SenderThread::main() {
  logger_->info("Starting sender.");
	node_->sender();
}


