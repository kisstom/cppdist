#include <cstdlib>
#include <cmath>
#include "store_from_binary_factory.h"

StoreFromBinary* StoreFromBinaryFactory::createStoreFromBinary(unordered_map<string, string>* params) {
  StoreFromBinary* storeFromBinary = new StoreFromBinary;
  bool isMulticast = false;
  int numSockets;
  int numSlaves = atoi((*params)["NUM_SLAVES"].c_str());
  int send_limit;

  if (params->find("MULTI") != params->end()) {
    isMulticast = atoi((*params)["MULTI"].c_str());
  }

  if (isMulticast) {
    numSockets = (int) pow(2, numSlaves - 1) - 1;
  } else {
    numSockets = numSlaves;
  }

  sscanf((*params)["SEND_LIMIT"].c_str(), "%d", &send_limit);

  storeFromBinary->resizeSocketNum(numSockets);
  storeFromBinary->setBufferCapacity(send_limit * 2);

  return storeFromBinary;
}
