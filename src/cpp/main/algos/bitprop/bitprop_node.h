/*
 * bitprop_node.h
 *
 *  Created on: 2014.08.11.
 *      Author: kisstom
 */

#ifndef BITPROP_NODE_H_
#define BITPROP_NODE_H_

#include <string>
#include <vector>

#include "../algo_components/old_partition_node.h"
#include "../../common/graph/edgelist_container.h"
#include "../../common/components/mutex.h"
#include "failed_estimate.h"
#include "IEstimationHandler.h"
#include "../../common/graph/iedge_list_builder.h"
#include "../algo_components/factories/graph_partition_config_handler.h"
#include <gtest/gtest_prod.h>

using std::string;

class BitpropNode: public OldPartitionNode {
public:
  BitpropNode(int, int, double, long);
  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void initFromMaster(string);
  void final();
  void initData(string);

  void serializeRandomBits(long, long, int);
  void estimate();
  void initBuffers();
  void updateBuffers();
  void updateBits(long outdge, unsigned char*);
  int numCodingOnes(long node);
  void findFirstLastIndices(std::vector<FailedEstimate>* failedEstimatedNodes, int* first, int* second);
  void getEstimation(int ones, double* est, bool* sing);

  // setters
  void setFailedEstimateNodes(std::vector<FailedEstimate>*);
  void setEstimatonHandler(IEstimationHandler*);
  void setContainer(EdgelistContainer*);
  void setRandomBits(unsigned char*);
  void setPartitionConfigHandler(GraphPartitionConfigHandler*);

  ~BitpropNode() {}

private:
  unsigned char* randomVectorBits;
  unsigned char* aux;
  unsigned char* temp;

  EdgelistContainer* matrix;
  IEdgeListBuilder* edgeListBuilder;
  Mutex mutex;
  long minnode;
  int numCodingBytes;
  int estIndex;
  double epsilon;
  short neighborhoodDistance;

  GraphPartitionConfigHandler* partConfHandler;
  std::vector<FailedEstimate>* failedEstimatedNodes;
  IEstimationHandler* estimationHandler;

  log4cpp::Category* logger;

  // TEST CASES
  FRIEND_TEST(FindFirstAndLastTest, test);
  FRIEND_TEST(FindFirstAndLastTest, test2);
  FRIEND_TEST(FindFirstAndLastTest, test3);
  FRIEND_TEST(FindFirstAndLastTest, test4);
};

#endif /* BITPROP_NODE_H_ */
