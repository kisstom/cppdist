#include "test_edge_list_builder.h"
#include <cstdio>
#include <cstdlib>

EdgelistContainer* TestEdgeListBuilder::buildFromString(vector<string> partString, long minNode) {
  vector<vector<long> > part = toMatrix(partString);

  long numNodes = (long) part.size();

  EdgelistContainer* container = new EdgelistContainer;
  container->setMinnode(minNode);
  container->initContainers();
  for (int start = 0; start < (int) part.size(); ++start) {
    for (int i = 0; i < part[start].size(); ++i) {
      container->addEdge(start + minNode, part[start][i]);
    }
    if (part[start].size() == 0) {
      container->addSinkPart(start);
    }
  }
  container->setFinish();

  return container;
}

vector<vector<long> > TestEdgeListBuilder::toMatrix(vector<string> stringM) {
  vector<vector<long> > retval;
  retval.resize(stringM.size());
  char node[1024];

  for (int i = 0; i < (int) stringM.size(); ++i) {
    std::stringstream row(stringM[i]);
    while (row.peek() != EOF) {
      row >> node;
      retval[i].push_back(atol(node));
    }
  }

  return retval;
}
