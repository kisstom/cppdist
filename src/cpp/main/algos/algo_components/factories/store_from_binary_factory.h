#ifndef STORE_FROM_BINARY_FACTORY_H_
#define STORE_FROM_BINARY_FACTORY_H_

#include "../../../common/components/store_from_binary.h"
#include <tr1/unordered_map>
#include <string>

using std::tr1::unordered_map;
using std::string;


class StoreFromBinaryFactory {
public:
  StoreFromBinary* createStoreFromBinary(unordered_map<string, string>* params);
private:
  int getBufferNum(unordered_map<string, string>* params);
};


#endif  // STORE_FROM_BINARY_FACTORY_H_
