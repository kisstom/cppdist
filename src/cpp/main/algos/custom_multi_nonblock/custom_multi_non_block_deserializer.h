/*
 * custom_non_block_deserializer.h
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#ifndef CUSTOM_MULTI_NON_BLOCK_DESERIALIZER_H_
#define CUSTOM_MULTI_NON_BLOCK_DESERIALIZER_H_

#include "custom_multi_non_block_node.h"
#include "../../common/components/deserializer.h"


class CustomMultiNonBlockDeserializer :public Deserializer {
public:
  CustomMultiNonBlockDeserializer();
  bool checkReadable(char* buffer, unsigned length);
  void setNode(CustomMultiNonBlockNode*);
  void update(short partindex);
  int storeFromBinary(char*, unsigned);

private:
  log4cpp::Category* logger_;
  CustomMultiNonBlockNode* node_;
  double imp;
  long fromNode;
};



#endif /* CUSTOM_MULTI_NON_BLOCK_DESERIALIZER_H_ */
