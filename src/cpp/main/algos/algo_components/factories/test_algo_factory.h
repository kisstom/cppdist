#ifndef TEST_ALGO_FACTORY_H_
#define TEST_ALGO_FACTORY_H_

#include "ialgo_factory.h"

class TestAlgoFactory : public IAlgoFactory {
public:
  AlgoBase* createAlgo(unordered_map<string, string>* params);
  void setPartitionMinNodes(vector<long>*);
private:
  vector<long>* partitionMinNodes;
};


#endif  // TEST_ALGO_FACTORY_H_
