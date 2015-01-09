#ifndef PARTITIONER_H_
#define PARTITIONER_H_

#include <vector>

using std::vector;

class Partitioner {
public:
  short getPartitionIndex(long key);
  void setPartMinNodes(vector<long>*);
private:
  vector<long>* partition_min_node_;
};


#endif  // PARTITIONER_H_
