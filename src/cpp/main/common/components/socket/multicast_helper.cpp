#include "multicast_helper.h"
#include <cmath>

int MulticastHelper::hash(short* outIndices) {
  short size = *outIndices;
  int hash = 0;
  for (short i = 0; i < size; ++i) {
    hash += pow(2, *(outIndices + i + 1));
  }

  return hash;
}
