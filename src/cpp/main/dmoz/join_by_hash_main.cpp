#include <iostream>
#include "join_by_hash.h"

int main(int argc, char* argv[]) {
  HashJoiner joiner;
  joiner.readHash2Topic(argv[1]);
  joiner.merge(argv[2], argv[3]);
}
