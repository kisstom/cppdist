/*
 * inner_master.cpp
 *
 *  Created on: 2014.06.13.
 *      Author: kisstom
 */

#include "inner_master.h"

void InnerMaster::addInfoForNodes(char* ss) {
  master_->sendMessageForAllNodes("");
}

void InnerMaster::setMaster(Master* master) {
  master_ = master;
}

