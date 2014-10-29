#ifndef PAGERANK_NON_BLOCK_DESERIALIZER_H_
#define PAGERANK_NON_BLOCK_DESERIALIZER_H_

#include "../../common/components/deserializer.h"
#include "pagerank_non_block_node.h"

class PagerankNonBlockDeserializer : public Deserializer {
public:
  PagerankNonBlockDeserializer();
  void update(short partindex);
  int storeFromBinary(char*, unsigned);
  bool checkReadable(char* buffer, unsigned length);
  void setNode(PagerankNonBlockNode*);
private:
  long to;
  double val;
  PagerankNonBlockNode* node_;
  log4cpp::Category* logger_;
};


#endif  // PAGERANK_NON_BLOCK_DESERIALIZER_H_
