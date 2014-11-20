#include "ip_index_maker.h"
#include <cmath>
#include <iostream>

using std::cout;

IPIndexMaker::IPIndexMaker(int _nodeIndex, int _clusterSize) {
  nodeIndex = _nodeIndex;
  clusterSize = _clusterSize;
  init();
}

void IPIndexMaker::setUpperBitExcluder() {
  int tmp = 0;
  for (int i = 0; i < nodeIndex; ++i) {
    tmp |= (1 << i);
  }

  upperBitExcluder = tmp;
}

void IPIndexMaker::setLowerBitExcluder() {
  int tmp = pow(2, clusterSize - 1) - 1;
  for (int i = 0; i < nodeIndex; ++i) {
    tmp ^= (1 << i);
  }

  lowerBitExcluder = tmp;
}

void IPIndexMaker::init() {
  setLowerBitExcluder();
  setUpperBitExcluder();
}

int IPIndexMaker::createPublisherIPIndex(int publisherIndex) {
  int retval = publisherIndex & lowerBitExcluder;
  retval <<= 1;
  retval |= publisherIndex & upperBitExcluder;
  return retval;
}

int IPIndexMaker::createClientIPIndex(int clientIndex) {
  int retval = clientIndex & lowerBitExcluder;

  retval <<= 1;
  retval |= (1 << nodeIndex);
  retval |= clientIndex & upperBitExcluder;

  return retval;
}
