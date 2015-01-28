#include <cstdlib>
#include <cmath>
#include "sender_buffer_factory.h"


SenderBuffer* SenderBufferFactory::createSenderBuffer(unordered_map<string, string>* params) {
  int numSockets = getBufferNum(params);
  int send_limit = sscanf((*params)["SEND_LIMIT"].c_str(),
      "%d", &send_limit);

  SenderBuffer* senderBuffer = new SenderBuffer;
  senderBuffer->resizeBufferNum(numSockets);
  senderBuffer->resizeBuffers(send_limit);

  return senderBuffer;
}

int SenderBufferFactory::getBufferNum(unordered_map<string, string>* params) {
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
