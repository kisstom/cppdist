/*
 * bitprop_deserializer.h
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#ifndef BITPROP_DESERIALIZER_H_
#define BITPROP_DESERIALIZER_H_

#include "../../common/components/deserializer.h"
#include "bitprop_node.h"

class BitpropDeserializer: public Deserializer {
public:
  BitpropDeserializer(int);

  void update(short partindex);
  int storeFromBinary(char*, unsigned);

  bool checkReadable(char* buffer, unsigned length);
  void setNode(BitpropNode*);

  ~BitpropDeserializer() {}

private:
  BitpropNode* node;
  int numCodingBytes;
  long to;
  unsigned char* codingBytes;
};


#endif /* BITPROP_DESERIALIZER_H_ */
