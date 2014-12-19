/*
 * inner_master.cpp
 *
 *  Created on: 2014.06.13.
 *      Author: kisstom
 */

#include "inner_master.h"

InnerMaster::InnerMaster() {
  logger_ = &log4cpp::Category::getInstance(std::string("InnerMaster"));
}

void InnerMaster::addInfoForNodes(char* ss) {
  logger_->info("Default inner master addinfo");
}

bool InnerMaster::nextIter() {
  logger_->info("Default inner master nextIter.");
  master_->sendMessageForAllNodes("");
  return true;
}

void InnerMaster::setMaster(MasterBase* master) {
  master_ = master;
}

