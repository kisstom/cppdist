#include "test_algo_factory.h"
#include "algo_factory_helper.h"

AlgoBase* TestAlgoFactory::createAlgo(unordered_map<string, string>* params) {
  AlgoFactoryHelper helper;
  AlgoBase* retval = helper.initAlgo(params);
  return retval;
}
