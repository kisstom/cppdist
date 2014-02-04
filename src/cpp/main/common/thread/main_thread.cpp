/*
 * main_thread.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "main_thread.h"

MainThread::MainThread(Runnable* runnable) {
	runnable_ = runnable;
	logger_ = &log4cpp::Category::getInstance(std::string("MainThread"));
}

void MainThread::main() {
	runnable_->run();
}


