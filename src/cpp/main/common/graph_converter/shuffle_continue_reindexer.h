#ifndef SHUFFLE_CONTINUE_REINDEXER_H_
#define SHUFFLE_CONTINUE_REINDEXER_H_

#include <string>
#include <tr1/unordered_map>
#include <gtest/gtest_prod.h>
#include "../util/util.h"

using std::string;
using std::tr1::unordered_map;

class ShuffleContinueReindexer {
public:
  ShuffleContinueReindexer(string, string, string, long numNodes, int numParts);
  void init();
  void run();
private:
  long reindexer(long);

  FILE* inputGraph;
  FILE* outputGraph;
  FILE* reindexFile;

  unordered_map<long, long> newIndices;
  string inputGraphFile;
  string outputGraphFile;
  string newIndicesFile;

  int numParts;
  long numNodes;
  long nodeCounter;
  long numNodePerPart;
  bool samePartSize;

  FRIEND_TEST(ShuffleContinueReindexerTest, test);
  FRIEND_TEST(ShuffleContinueReindexerTest, test2);
  FRIEND_TEST(ShuffleContinueReindexerTest, test3);
};


#endif  // SHUFFLE_CONTINUE_REINDEXER_H_
