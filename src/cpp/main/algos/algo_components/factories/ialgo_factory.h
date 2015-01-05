#ifndef IALGO_FACTORY_H_
#define IALGO_FACTORY_H_

#include <tr1/unordered_map>
#include <string>
#include <vector>
#include "../algo_base/algo_base.h"

class IAlgoFactory {
public:
  virtual AlgoBase* createAlgo(unordered_map<string, string>* params) = 0;
};


#endif  // IALGO_FACTORY_H_
