#ifndef ALS_DESERIALIZER_H_
#define ALS_DESERIALIZER_H_

#include "../../common/components/deserializer.h"
#include "als_node.h"
#include "log4cpp/Category.hh"

class AlsDeserializer : public Deserializer {
public:
  AlsDeserializer(int featSize);
  void update(short partindex);
  int storeFromBinary(char*, unsigned);

  bool checkReadable(char* buffer, unsigned length);
  void setNode(AlsNode*);

  ~AlsDeserializer();

private:
  double* feats;
  long key;
  int numFeat;
  AlsNode* node_;
  log4cpp::Category* logger_;
};


#endif  // ALS_DESERIALIZER_H_
