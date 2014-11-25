#include "multicast_helper.h"
#include <cmath>

MulticastHelper::MulticastHelper(int _nodeIndex) {
  nodeIndex = _nodeIndex;
}

int MulticastHelper::hash(short* outIndices) {
  short size = *outIndices;
  int hash = 0;
  short value;
  for (short i = 0; i < size; ++i) {
    value = *(outIndices + i + 1);

    if (value == nodeIndex) continue;
    if (value > nodeIndex) --value;

    hash |= (1 << value);
  }

  return hash - 1;
}
