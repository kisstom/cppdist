/*
 * bitprop_master.cpp
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#include "bitprop_master.h"

BitpropMaster::BitpropMaster(short _neighborhoodSize) {
  neighborhoodSize = _neighborhoodSize;
  neighborhoodDistance = 0;
}

// This logic could be moved to nodes.
bool BitpropMaster::nextIter() {
  master_->sendMessageForAllNodes("");
  neighborhoodDistance++;

  if (neighborhoodDistance >= neighborhoodSize) {
    return false;
  }

  return true;
}
