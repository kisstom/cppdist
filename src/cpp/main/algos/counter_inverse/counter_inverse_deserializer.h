/*
 * counter_inverse_deserializer.h
 *
 *  Created on: 2014.09.23.
 *      Author: kisstom
 */

#ifndef COUNTER_INVERSE_DESERIALIZER_H_
#define COUNTER_INVERSE_DESERIALIZER_H_

#include "counter_inverse_node.h"
#include "../../common/components/deserializer.h"

class CounterInverseDeserializer :public Deserializer {
public:
  CounterInverseDeserializer();
  bool checkReadable(char* buffer, unsigned length);
  void setNode(CounterInverseNode*);
  void update(short partindex);
  int storeFromBinary(char*, unsigned);

private:
  log4cpp::Category* logger_;
  CounterInverseNode* node_;
  long to;
  long from;
};


#endif /* COUNTER_INVERSE_DESERIALIZER_H_ */
