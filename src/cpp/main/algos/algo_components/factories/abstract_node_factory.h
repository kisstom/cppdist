#ifndef ABSTRACT_NODE_FACTORY_H_
#define ABSTRACT_NODE_FACTORY_H_

#include "../inode_factory.h"
#include <log4cpp/Category.hh>

class AbstractNodeFactory {
public:
  AbstractNodeFactory();
  INodeFactory* provideNodeFactory(unordered_map<string, string>* params);
private:
  log4cpp::Category* logger_;
};


#endif  // ABSTRACT_NODE_FACTORY_H_
