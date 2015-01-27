#ifndef TEST_EDGE_LIST_BUILDER_H_
#define TEST_EDGE_LIST_BUILDER_H_

#include <string>
#include <vector>
#include "../edgelist_container.h"

using std::string;
using std::vector;

class TestEdgeListBuilder {
public:
  EdgelistContainer* buildFromString(vector<string>, long);
  vector<vector<long> > toMatrix(vector<string> stringM);
};


#endif  // TEST_EDGE_LIST_BUILDER_H_
