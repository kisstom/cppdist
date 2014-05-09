/*
 * pagerank_serializer.h
 *
 *  Created on: 2014.05.09.
 *      Author: kisstom
 */

#ifndef PAGERANK_DESERIALIZER_H_
#define PAGERANK_DESERIALIZER_H_

#include "../../common/components/deserializer.h"
#include "pagerank_node.h"

class PagerankDeserializer : public Deserializer {
public:
  PagerankDeserializer();
  void update(short partindex);
  int storeFromBinary(char*, unsigned);
  bool checkReadable(char* buffer, unsigned length);
  void setNode(PagerankNode*);
private:
  long to;
  double val;
  PagerankNode* node_;
  log4cpp::Category* logger_;
};


#endif /* PAGERANK_DESERIALIZER_H_ */
