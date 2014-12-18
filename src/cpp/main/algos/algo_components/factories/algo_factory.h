#ifndef ALGO_FACTORY_H_
#define ALGO_FACTORY_H_

#include <tr1/unordered_map>
#include <string>

using std::tr1::unordered_map;
using std::string;

class AlgoFactory {
public:
  AlgoBase* createAlgo(unordered_map<string, string>* params);
};


#endif  // ALGO_FACTORY_H_
