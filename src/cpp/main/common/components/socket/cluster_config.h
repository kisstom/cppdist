#ifndef CLUSTER_CONFIG_H_
#define CLUSTER_CONFIG_H_

#include <vector>
#include <cstdlib>

using std::vector;

class ClusterConfig {
public:
  char* getHost(int);
  int getPort(int);
  void initClusterSize(int);
  void setHost(char* host, int i);
  void setPort(int port, int i);

private:
  vector<char*> hosts;
  vector<int> ports;
  int clusterSize;

};


#endif  // CLUSTER_CONFIG_H_
