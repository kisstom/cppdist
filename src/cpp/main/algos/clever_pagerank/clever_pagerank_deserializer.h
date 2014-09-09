/*
 * clever_pagerank_deserializer.h
 *
 *  Created on: 2014.09.09.
 *      Author: kisstom
 */

#ifndef CLEVER_PAGERANK_DESERIALIZER_H_
#define CLEVER_PAGERANK_DESERIALIZER_H_

#include "clever_pagerank_node.h"
#include "../../common/components/deserializer.h"


class CleverPagerankDeserializer :public Deserializer {
public:
  CleverPagerankDeserializer();
  bool checkReadable(char* buffer, unsigned length);
  void setNode(CleverPagerankNode*);
  void update(short partindex);
  int storeFromBinary(char*, unsigned);

private:
  CleverPagerankNode* node_;
  double imp;
  long fromNode;
};


#endif /* CLEVER_PAGERANK_DESERIALIZER_H_ */
