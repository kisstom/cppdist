#include "multicast_helper.h"
#include <cmath>

MulticastHelper::MulticastHelper(int _nodeIndex) {
  nodeIndex = _nodeIndex;

  logger_ = &log4cpp::Category::getInstance(std::string("MulticastHelper"));
  logger_->info("node_index %d", nodeIndex);
}

int MulticastHelper::publishHashId(short* outIndices, bool* shouldUpdateSelf) {
  short size = *outIndices;
  int hash = 0;
  short value;
  *shouldUpdateSelf = false;
  for (short i = 0; i < size; ++i) {
    value = *(outIndices + i + 1);

    if (value == nodeIndex) {
      *shouldUpdateSelf = true;
      continue;
    }
    if (value > nodeIndex) --value;

    hash |= (1 << value);
  }

  return hash - 1;
}


int MulticastHelper::publishHashId(const set<short>& outIds, bool* shouldUpdateSelf) {
  int hash = 0;
  short value;
  *shouldUpdateSelf = false;
  for (set<short>::const_iterator it = outIds.begin(); it != outIds.end(); ++it) {
    value = *it;

    //logger_->info("value %hd node_index %d", value, nodeIndex);
    if (value == nodeIndex) {
      *shouldUpdateSelf = true;
      continue;
    }
    if (value > nodeIndex) --value;

    hash |= (1 << value);
  }

  return hash - 1;
}
