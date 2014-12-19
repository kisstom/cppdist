#include <cstdlib>
#include <cmath>
#include "sender_buffer_factory.h"


SenderBuffer* SenderBufferFactory::createSenderBuffer(unordered_map<string, string>* params) {
  bool isMulticast;
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

  SenderBuffer* senderBuffer_ = new SenderBuffer;
  senderBuffer_->resizeBufferNum(numSockets);
  senderBuffer_->resizeBuffers(send_limit);

  return senderBuffer_;
}
