#include "cluster_config.h"
#include <cstring>

char* ClusterConfig::getHost(int i) {
  return hosts[i];
}

int ClusterConfig::getPort(int i) {
  return ports[i];
}

void ClusterConfig::initClusterSize(int csize) {
  hosts.resize(csize);
  ports.resize(csize);

  for (int i = 0; i < csize; ++i) {
    hosts[i] = new char[1024];
  }
}

void ClusterConfig::setHost(const char* host, int i) {
  strcpy(hosts[i], host);
}

void ClusterConfig::setPort(int port, int i) {
  ports[i] = port;
}
