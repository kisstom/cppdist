#ifndef CLUSTER_CONFIG_H_
#define CLUSTER_CONFIG_H_

#include <cstdlib>

class ClusterConfig {
public:
  char* getHost(int);
  int getPort(int);
};


#endif  // CLUSTER_CONFIG_H_
