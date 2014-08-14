/*
 * node_factory.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "node_factory.h"
#include "../bitprop/random_bitvector_generator.h"
#include "../../common/graph/edge_list_container_factory.h"

NodeFactory::NodeFactory() {
	logger_ = &log4cpp::Category::getInstance(std::string("NodeFactory"));
}

Node* NodeFactory::createNodeFromConfig(unordered_map<string, string>* params) {
	string nodeType = (*params)["NODE_TYPE"];
	Node* node = NULL;
	if (nodeType.compare("SIMRANK_UPDATE") == 0) {
		node = NULL;
	} else if (nodeType.compare("SIMRANK_STORE_FIRST") == 0) {
		node = createSimrankStoreFirstNode(params);
	} else if (nodeType.compare("SIMRANK_ODD_EVEN") == 0) {
		node = createSimrankOddEvenNode(params);
	} else if (nodeType.compare("PAGERANK") == 0) {
	  node = createPagerankNode(params);
	} else if (nodeType.compare("PSIMRANK") == 0) {
    node = createPSimrankNode(params);
  } else if (nodeType.compare("BITPROP") == 0) {
    node = createBitpropNode(params);
  } else {
		logger_->error("ERROR. Unknown type of algo %s.\n", nodeType.c_str());
	}
	return node;
}

SimrankStoreFirstNode* NodeFactory::createSimrankStoreFirstNode(
		unordered_map<string, string>* params) {
  string fpStartName = (*params)["FP_START_NAME"];
  char outputFileN[1024];
  sprintf(outputFileN, "%sout_%s", (*params)["LOCAL_DIR"].c_str(), (*params)["SLAVE_INDEX"].c_str());

  short numPathes;
	short pathLen;
	sscanf((*params)["NUM_PATHES"].c_str(), "%hd", &numPathes);
	sscanf((*params)["PATH_LEN"].c_str(), "%hd", &pathLen);

	SimrankStoreFirstNode* node = new SimrankStoreFirstNode(numPathes, pathLen, fpStartName, string(outputFileN));
  return node;
}

SimrankOddEvenNode* NodeFactory::createSimrankOddEvenNode(
		unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  SimrankOddEvenNode* node = helper.initSimrankOddEvenNode(params);
  IEdgeListBuilder* edgeListBuilder = createEdgeListBuilder(params);

  char outputFileN[1024];
  sprintf(outputFileN, "%sout_%s", (*params)["LOCAL_DIR"].c_str(), (*params)["SLAVE_INDEX"].c_str());
  node->setEdgeListBuilder(edgeListBuilder);
  node->setOutputFile(string(outputFileN));
  node->setFingerPrintFile((*params)["FP_START_NAME"]);
  node->initData((*params)["INPUT_PARTITION"]);
  return node;
}

PagerankNode* NodeFactory::createPagerankNode(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  EdgelistContainer* container = createEdgeListContainer(params);
  PagerankNode* node = helper.initPagerankNode(params);
  node->setEdgeListContainer(container);

  char outputFileN[1024];
  sprintf(outputFileN, "%sout_%s", (*params)["LOCAL_DIR"].c_str(), (*params)["SLAVE_INDEX"].c_str());
  node->setOutputFile(string(outputFileN));
  return node;
}

PSimrankNode* NodeFactory::createPSimrankNode(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  PSimrankNode* node = helper.initPSimrankNode(params);
  IEdgeListBuilder* edgeListBuilder = createEdgeListBuilder(params);

  char outputFileN[1024];
  sprintf(outputFileN, "%sout_%s", (*params)["LOCAL_DIR"].c_str(), (*params)["SLAVE_INDEX"].c_str());
  node->setEdgeListBuilder(edgeListBuilder);
  node->setOutputFile(string(outputFileN));
  node->setFingerPrintFile((*params)["FP_START_NAME"]);
  node->initData((*params)["INPUT_PARTITION"]);
  return node;
}

BitpropNode* NodeFactory::createBitpropNode(unordered_map<string, string>* params) {
  NodeFactoryHelper helper;
  BitpropNode* node = helper.initBitpropNode(params);

  EdgeListContainerFactory edgeListContainerFactory;
  EdgelistContainer* container = edgeListContainerFactory.createEdgeListContainer(params);
  node->setContainer(container);

  std::vector<FailedEstimate>* failedEstimatedNodes = readFailedEstimations(params);
  node->setFailedEstimateNodes(failedEstimatedNodes);

  EstimationHandler* estimationHandler = createEstimationHandler(params);
  node->setEstimatonHandler(estimationHandler);

  util.checkParam(params, 3, "NUM_NODES", "NUM_CODING_BYTES", "EPSILON");
  long numNodes;
  int numCodingBytes;
  double epsilon;

  sscanf((*params)["NUM_NODES"].c_str(), "%ld", &numNodes);
  sscanf((*params)["NUM_CODING_BYTES"].c_str(), "%d", &numCodingBytes);
  sscanf((*params)["EPSILON"].c_str(), "%lf", &epsilon);

  logger_->info("epsilon %lf", epsilon);
  unsigned char* randomVectorBits = initRandomVectorBits(numNodes, numCodingBytes, epsilon);
  node->setRandomBits(randomVectorBits);
  node->initBuffers();

  return node;
}

unsigned char* NodeFactory::initRandomVectorBits(long numNodes, int numCodingBytes, double epsilon) {
  RandomBitvectorGenerator rvbgen(epsilon, 13);

  unsigned char* randomVectorBits = new unsigned char[numNodes * numCodingBytes];
  for (long node = 0; node < numNodes; ++node) {
    rvbgen.gen(numCodingBytes, randomVectorBits + node * numCodingBytes);
  }

  return randomVectorBits;
}

EstimationHandler* NodeFactory::createEstimationHandler(unordered_map<string, string>* params) {
  logger_->info("Creating estimation handler.");

  util.checkParam(params, 3, "LOCAL_DIR", "NEIGHBORHOOD_SIZE", "SLAVE_INDEX");
  string baseDir = (*params)["LOCAL_DIR"];
  int slaveIndex;
  sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slaveIndex);

  short neighborhoodSize;
  sscanf((*params)["NEIGHBORHOOD_SIZE"].c_str(), "%hd", &neighborhoodSize);

  EstimationHandler* estimationHandler = new EstimationHandler(baseDir, neighborhoodSize, slaveIndex);
  return estimationHandler;
}


EdgelistContainer* NodeFactory::createEdgeListContainer(unordered_map<string, string>* params) {
  IEdgeListBuilder* builder = createEdgeListBuilder(params);
  long min_node;
  sscanf((*params)["MIN_NODE"].c_str(), "%ld", &min_node);
  logger_->info("Initing edge list container.");

  EdgelistContainer* matrix = new EdgelistContainer();
  matrix->initContainers();
  matrix->setMinnode(min_node);

  builder->setContainer(matrix);
  builder->buildFromFile((*params)["INPUT_PARTITION"]);

  logger_->info("Edge list container inited.");
  return matrix;
}

IEdgeListBuilder* NodeFactory::createEdgeListBuilder(unordered_map<string, string>* params) {
  IEdgeListBuilder* edgeListBuilder = NULL;

  if (params->find("USE_PREPROCESS") == params->end()) {
    edgeListBuilder = new EdgeListBuilder;
  } else {
    if ((*params)["USE_PREPROCESS"].compare("CRAWL") == 0) {
      long maxNodeToKeep;
      sscanf((*params)["MAX_NODE_TO_KEEP"].c_str(), "%ld", &maxNodeToKeep);
      CrawlEdgeListBuilder* crawlEdgeListBuilder = new CrawlEdgeListBuilder(
          maxNodeToKeep);
      edgeListBuilder = crawlEdgeListBuilder;
    } else if ((*params)["USE_PREPROCESS"].compare("FILTER") == 0) {
      FilterEdgeListBuilder* filterEdgeListBuilder = new FilterEdgeListBuilder;
      string nodesToDeleteFile = (*params)["FILTER_NODE_FILE"];
      filterEdgeListBuilder->readNodesToDelete(nodesToDeleteFile);
      edgeListBuilder = filterEdgeListBuilder;
    } else {
      logger_->error("ERROR. Unknown type of preprcessing %s.\n",
          (*params)["USE_PREPROCESS"].c_str());
      return NULL;
    }
  }

  return edgeListBuilder;
}

std::vector<FailedEstimate>* NodeFactory::readFailedEstimations(unordered_map<string, string>* params) {
  logger_->info("Reading previous failed estimationns.");
  util.checkParam(params, 4, "PREV_OUTDIR", "EST_INDEX", "NEIGHBORHOOD_SIZE", "SLAVE_INDEX");
  int estIndex, slaveIndex;
  short neighborhoodSize;
  string prevOutdir;

  sscanf((*params)["NEIGHBORHOOD_SIZE"].c_str(), "%hd", &neighborhoodSize);
  sscanf((*params)["SLAVE_INDEX"].c_str(), "%d", &slaveIndex);
  sscanf((*params)["EST_INDEX"].c_str(), "%d", &estIndex);
  prevOutdir = (*params)["PREV_OUTDIR"];

  if (estIndex == 0) return NULL;

  long nodeId;
  double value;

  std::vector<FailedEstimate>* failedEstimations = new std::vector<FailedEstimate>();
  for (short currentDistance = 0; currentDistance < neighborhoodSize; ++currentDistance) {
    logger_->info("Reading previous failed estimations at distance %hd", currentDistance);
    stringstream s;
    s << prevOutdir << "failed_estimates_distance_" << currentDistance << "_part_" << slaveIndex;
    string last_failed_estimate_file = s.str();

    FILE* last_failed_estimate = fopen(last_failed_estimate_file.c_str(), "r");
    if (last_failed_estimate == NULL) {
      throw IOError("Failed opening failed estimate file: " + last_failed_estimate_file);
    }

    int numLastSingular = 0;
    while (fscanf(last_failed_estimate, "%ld %lf\n", &nodeId, &value) != EOF) {
      failedEstimations->push_back(FailedEstimate(value, nodeId, currentDistance));
      if (value == -1) ++numLastSingular;
    }

    logger_->info("Previous failed estimations read. Size : %d", (int) failedEstimations->size());
    logger_->info("Number of singular among them: %d", numLastSingular);
    fclose(last_failed_estimate);
  }

  return failedEstimations;
}
