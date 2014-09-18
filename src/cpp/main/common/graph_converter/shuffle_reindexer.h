/*
 * part_index_reindexer.h
 *
 *  Created on: 2014.09.18.
 *      Author: kisstom
 */

#ifndef SHUFFLE_REINDEXER_H_
#define SHUFFLE_REINDEXER_H_

#include <string>
#include <tr1/unordered_map>
#include "../util/util.h"

using std::string;
using std::tr1::unordered_map;

class ShuffleReindexer {
public:
  ShuffleReindexer(string, string, string);
  void init();
  void run();
  void readShuffleIndices(FILE*);
private:
  FILE* inputGraph;
  FILE* outputGraph;
  FILE* shuffleIndices;

  unordered_map<long, long> shuffleHash;
  string inputGraphFile;
  string outputGraphFile;
  string shuffleIndicesFile;

  int rowlen;
  Util util;
};

#endif /* SHUFFLE_REINDEXER_H_ */
