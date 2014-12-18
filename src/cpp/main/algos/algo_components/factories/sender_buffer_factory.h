#ifndef SENDER_BUFFER_FACTORY_H_
#define SENDER_BUFFER_FACTORY_H_

#include <tr1/unordered_map>
#include <string>

using std::tr1::unordered_map;
using std::string;

class SenderBufferFactory {
public:
  SenderBuffer* createSenderBuffer(unordered_map<string, string>* params);
};


#endif  // SENDER_BUFFER_FACTORY_H_
