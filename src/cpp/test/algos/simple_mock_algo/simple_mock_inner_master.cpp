/*
 * simple_mock_inner_master.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "simple_mock_inner_master.h"

SimpleMockInnerMaster::SimpleMockInnerMaster() {
	logger_ = &log4cpp::Category::getInstance(std::string("SimpleMockInnerMaster"));
	iteration_ = 0;
}

bool SimpleMockInnerMaster::nextIter() {
  logger_->info("Next iteration.");
	++iteration_;
	if (iteration_ >= 2) return false;
	return true;
}

void SimpleMockInnerMaster::addInfoForNodes(string* ss) {

}


