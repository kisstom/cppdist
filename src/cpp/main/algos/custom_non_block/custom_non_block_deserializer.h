/*
 * custom_non_block_deserializer.h
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#ifndef CUSTOM_NON_BLOCK_DESERIALIZER_H_
#define CUSTOM_NON_BLOCK_DESERIALIZER_H_

#include "custom_non_block_node.h"
#include "../../common/components/deserializer.h"


class CustomNonBlockDeserializer :public Deserializer {
public:
  CustomNonBlockDeserializer();
  bool checkReadable(char* buffer, unsigned length);
  void setNode(CustomNonBlockNode*);
  void update(short partindex);
  int storeFromBinary(char*, unsigned);
  ~CustomNonBlockDeserializer();

private:
  int messageCounter;
  log4cpp::Category* logger_;
  CustomNonBlockNode* node_;
  double imp;
  long fromNode;
};



#endif /* CUSTOM_NON_BLOCK_DESERIALIZER_H_ */
