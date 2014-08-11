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

bool BitpropMaster::nextIter() {
  neighborhoodDistance++;

  if (neighborhoodDistance >= neighborhoodSize) {
    return false;
  }

  return true;
}
