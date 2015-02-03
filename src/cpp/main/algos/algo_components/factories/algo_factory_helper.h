#ifndef ALGO_FACTORY_HELPER_H_
#define ALGO_FACTORY_HELPER_H_

#include <tr1/unordered_map>
#include <string>
#include "../algo_base/algo_base.h"

class AlgoFactoryHelper {
public:
  AlgoFactoryHelper();
  AlgoBase* initAlgo(unordered_map<string, string>* params);
private:
  log4cpp::Category* logger_;
};


#endif  // ALGO_FACTORY_HELPER_H_
