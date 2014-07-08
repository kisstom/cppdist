/*
 * setup_thread.cpp
 *
 *  Created on: 2014.06.17.
 *      Author: kisstom
 */

#include "setup_thread.h"

SetupThread::SetupThread(Runnable* runnable) {
  runnable_ = runnable;
  logger_ = &log4cpp::Category::getInstance(std::string("SetupThread"));
}

void SetupThread::main() {
  runnable_->setUp();
}


