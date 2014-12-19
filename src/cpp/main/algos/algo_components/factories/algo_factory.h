#ifndef ALGO_FACTORY_H_
#define ALGO_FACTORY_H_

#include <tr1/unordered_map>
#include <string>
#include <vector>
#include "log4cpp/Category.hh"
#include "../algo_base/algo_base.h"

using std::tr1::unordered_map;
using std::string;
using std::vector;

class AlgoFactory {
public:
  AlgoFactory();
  AlgoBase* createAlgo(unordered_map<string, string>* params);
  vector<long>*  readSlaveConfig(unordered_map<string, string>* params);
private:
  log4cpp::Category* logger_;
};


#endif  // ALGO_FACTORY_H_
