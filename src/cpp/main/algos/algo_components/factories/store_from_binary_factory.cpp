#include <cstdlib>
#include <cmath>
#include "store_from_binary_factory.h"

StoreFromBinary* StoreFromBinaryFactory::createStoreFromBinary(unordered_map<string, string>* params) {
  int numSockets = getBufferNum(params);
  int send_limit = sscanf((*params)["SEND_LIMIT"].c_str(),
      "%d", &send_limit);

  StoreFromBinary* storeFromBinary = new StoreFromBinary;
  storeFromBinary->resizeSocketNum(numSockets);
  storeFromBinary->setBufferCapacity(send_limit * 2);

  return storeFromBinary;
}

int StoreFromBinaryFactory::getBufferNum(unordered_map<string, string>* params) {
  int numSockets = -1;
  int numSlaves = atoi((*params)["NUM_SLAVES"].c_str());

  if (params->find("MULTI") != params->end()) {
    if ((*params)["MULTI"].compare("MULTICAST") == 0) {
      numSockets = (int) pow(2, numSlaves - 1) - 1;
    } else if ((*params)["MULTI"].compare("BROADCAST") == 0) {
      numSockets = 1;
    } else {
      throw std::exception();
    }
  } else {
    numSockets = numSlaves;
  }

  return numSockets;
}

