#ifndef ADJACENCY_LIST_BUILDER_H_
#define ADJACENCY_LIST_BUILDER_H_

#include "../adjacency_list.h"
#include "../entry.h"
#include "log4cpp/Category.hh"

class AdjacencyListBuilder {
public:
  void buildFromFile(string, AdjacencyList<Entry>*);
  void buildTransposeFromFile(string, AdjacencyList<Entry>*);
private:
  log4cpp::Category* logger_;
};


#endif  // ADJACENCY_LIST_BUILDER_H_
