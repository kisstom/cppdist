#include "client_socket_manager_factory.h"

ClientSocketManagerFactory::ClientSocketManagerFactory() {
  logger = &log4cpp::Category::getInstance(std::string("ClientSocketManagerFactory"));
}

ClientSocketManager* ClientSocketManagerFactory::
createClientSocketManager(unordered_map<string, string>* params, vector<std::pair<string, string> >* hostAndPort) {
  logger->info("Creating client csm.");
  int slaveIndex;
  int numSlaves;
  int initCommPort;

  sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slaveIndex);
  sscanf((*params)["NUM_SLAVES"].c_str(), "%d", &numSlaves);
  sscanf((*params)["INIT_SLAVE_COMMUNICATION_PORT"].c_str(), "%d", &initCommPort);

  ClientSocketManager* clientSocketManager = new ClientSocketManager(slaveIndex, numSlaves);
  ClusterConfig* clusterConfig = NULL;

  clusterConfig = createClusterConfig(hostAndPort, initCommPort, numSlaves);
  clientSocketManager->setClusterConfig(clusterConfig);

  logger->info("Created client csm.");
  return clientSocketManager;
}

ClusterConfig* ClientSocketManagerFactory::createClusterConfig(
    vector<std::pair<string, string> >* hostAndPort, int initSlavePort, int numSlaves)  {
  ClusterConfig* config = new ClusterConfig;
  config->initClusterSize(numSlaves);

  int nodeIndex = 0;
  int nodeOnThisHost;
  char actIp[1024];

  for (vector<std::pair<string, string> >::iterator it = hostAndPort->begin();
      it != hostAndPort->end(); ++it) {
    nodeOnThisHost = atoi(it->second.c_str());
    util_.setIpByHost(it->first.c_str(), actIp);

    for (int i = 0; i < nodeOnThisHost; ++i) {
      config->setHost(actIp, nodeIndex);
      config->setPort(initSlavePort + nodeIndex, nodeIndex);
      ++nodeIndex;
    }
  }

  return config;
}

