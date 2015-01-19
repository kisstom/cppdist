#ifndef ABSTRACT_NODE_FACTORY_H_
#define ABSTRACT_NODE_FACTORY_H_

#include "../inode_factory.h"

class AbstractNodeFactory {
public:
  INodeFactory* provideNodeFactory(unordered_map<string, string>* params);
};


#endif  // ABSTRACT_NODE_FACTORY_H_
