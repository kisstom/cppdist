#ifndef ALS_MULTI_2STEP_DESERIALIZER_H_
#define ALS_MULTI_2STEP_DESERIALIZER_H_

#include "../../common/components/deserializer.h"
#include "als_multi_2step.h"
#include "log4cpp/Category.hh"

class AlsMulti2StepDeserializer : public Deserializer {
public:
  AlsMulti2StepDeserializer(int featSize);
  void update(short partindex);
  int storeFromBinary(char*, unsigned);

  bool checkReadable(char* buffer, unsigned length);
  void setNode(AlsMulti2Step*);

  ~AlsMulti2StepDeserializer();

private:
  int messageCounter;
  double* feats;
  long key;
  int numFeat;
  AlsMulti2Step* node_;
  log4cpp::Category* logger_;
};


#endif  // ALS_MULTI_2STEP_DESERIALIZER_H_
