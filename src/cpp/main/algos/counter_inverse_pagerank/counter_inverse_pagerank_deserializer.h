/*
 * counter_inverse_pagerank_deserializer.h
 *
 *  Created on: 2014.09.24.
 *      Author: kisstom
 */

#ifndef COUNTER_INVERSE_PAGERANK_DESERIALIZER_H_
#define COUNTER_INVERSE_PAGERANK_DESERIALIZER_H_

#include "counter_inverse_pagerank_node.h"
#include "../../common/components/deserializer.h"

class CounterInversePagerankDeserializer: public Deserializer {
public:
  CounterInversePagerankDeserializer();
  bool checkReadable(char* buffer, unsigned length);
  void setNode(CounterInversePagerankNode*);
  void update(short partindex);
  int storeFromBinary(char*, unsigned);

private:
  log4cpp::Category* logger_;
  CounterInversePagerankNode* node_;
  double importance;
};


#endif /* COUNTER_INVERSE_PAGERANK_DESERIALIZER_H_ */
