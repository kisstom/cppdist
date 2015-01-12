#include "node_factory_helper.h"


NodeFactoryHelper::NodeFactoryHelper() {
	logger_ = &log4cpp::Category::getInstance(std::string("NodeFactoryHelper"));
}

BitpropNode* NodeFactoryHelper::initBitpropNode(unordered_map<string, string>* params) {
  util.checkParam(params, 4, "NUM_CODING_BYTES", "EPSILON", "EST_INDEX");
  int numCodingBytes;
  double epsilon;
  int estIndex;

  sscanf((*params)["NUM_CODING_BYTES"].c_str(), "%d", &numCodingBytes);
  sscanf((*params)["EPSILON"].c_str(), "%lf", &epsilon);
  sscanf((*params)["EST_INDEX"].c_str(), "%d", &estIndex);

  reader.readSlaveConfig(params);
  long min_node = reader.getMinNodeFromParams(params);

  return new BitpropNode(numCodingBytes, estIndex, epsilon, min_node);
}


PagerankNode* NodeFactoryHelper::initPagerankNode(unordered_map<string, string>* params) {
  long allNode;
  int maxIter;
  double dump;

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &allNode);
  sscanf((*params)["MAX_ITER"].c_str(), "%d", &maxIter);
  sscanf((*params)["DUMP"].c_str(), "%lf", &dump);

  PagerankNode* node = new PagerankNode(maxIter, allNode, dump);
  return node;
}

PagerankNonBlockNode* NodeFactoryHelper::initPagerankNonBlockNode(unordered_map<string, string>* params) {
  long allNode, min_node;
  int maxIter;
  double dump;
  long min_node = reader.getMinNodeFromParams(params);

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &allNode);
  sscanf((*params)["MAX_ITER"].c_str(), "%d", &maxIter);
  sscanf((*params)["DUMP"].c_str(), "%lf", &dump);

  PagerankNonBlockNode* node = new PagerankNonBlockNode(allNode, min_node, dump, maxIter);
  return node;
}

SimrankOddEvenNode* NodeFactoryHelper::initSimrankOddEvenNode(unordered_map<string, string>* params) {
	GeneratorType type = SRAND;
	int seed;
	short numPathes;
	short pathLen;

	sscanf((*params)["NUM_PATHES"].c_str(), "%hd", &numPathes);
	sscanf((*params)["PATH_LEN"].c_str(), "%hd", &pathLen);

	if (params->find(string("SEED")) == params->end()) {
		seed = 13;
	} else {
		sscanf((*params)["SEED"].c_str(), "%d", &seed);
	}

	if (params->find(string("RANDOM_TYPE")) == params->end()) {
		type = SRAND;
	} else {
		if ((*params)["RANDOM_TYPE"].compare("PSEUDO") == 0) {
			type = PSEUDO_RANDOM;
		} else if ((*params)["RANDOM_TYPE"].compare("SRAND") == 0) {
			type = SRAND;
		} else {
			logger_->error("Error unknoyn tpye of random generator %s",
					(*params)["RANDOM_TYPE"].c_str());
		}
	}

	reader.readSlaveConfig(params);
	long min_node = reader.getMinNodeFromParams(params);
	long num_nodes = reader.getNumNodeFromParams(params);
	long next_min_node = reader.getNextMinNodeFromParams(params);


	SimrankOddEvenNode* node = new SimrankOddEvenNode(numPathes, pathLen, seed,
			type, num_nodes, min_node, next_min_node);
	return node;
}

PSimrankNode* NodeFactoryHelper::initPSimrankNode(unordered_map<string, string>* params) {
  GeneratorType type = SRAND;
  int seed;
  short numPathes;
  short pathLen;

  sscanf((*params)["NUM_PATHES"].c_str(), "%hd", &numPathes);
  sscanf((*params)["PATH_LEN"].c_str(), "%hd", &pathLen);

  reader.readSlaveConfig(params);
  long min_node = reader.getMinNodeFromParams(params);
  long num_nodes = reader.getNumNodeFromParams(params);
  long next_min_node = reader.getNextMinNodeFromParams(params);


  PSimrankNode* node = new PSimrankNode(numPathes, pathLen, num_nodes, min_node, next_min_node);
  return node;
}

CleverPagerankNode* NodeFactoryHelper::initCleverPagerankNode(unordered_map<string, string>* params) {
  long allNode;
  int maxIter;
  double dump;

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &allNode);
  sscanf((*params)["MAX_ITER"].c_str(), "%d", &maxIter);
  sscanf((*params)["DUMP"].c_str(), "%lf", &dump);

  reader.readSlaveConfig(params);
  long min_node = reader.getMinNodeFromParams(params);

  CleverPagerankNode* node = new CleverPagerankNode(allNode, minNode, dump, maxIter);
  return node;
}

CustomNonBlockNode* NodeFactoryHelper::initCustomNonBlockNode(unordered_map<string, string>* params) {
  long allNode;
  int maxIter;
  double dump;

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &allNode);
  sscanf((*params)["MAX_ITER"].c_str(), "%d", &maxIter);
  sscanf((*params)["DUMP"].c_str(), "%lf", &dump);

  reader.readSlaveConfig(params);
  long min_node = reader.getMinNodeFromParams(params);

  CustomNonBlockNode* node = new CustomNonBlockNode(allNode, minNode, dump, maxIter);
  return node;
}

CustomMultiNonBlockNode* NodeFactoryHelper::initCustomMultiNonBlockNode(unordered_map<string, string>* params) {
  long allNode;
  int maxIter;
  double dump;

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &allNode);
  sscanf((*params)["MAX_ITER"].c_str(), "%d", &maxIter);
  sscanf((*params)["DUMP"].c_str(), "%lf", &dump);

  reader.readSlaveConfig(params);
  long min_node = reader.getMinNodeFromParams(params);

  CustomMultiNonBlockNode* node = new CustomMultiNonBlockNode(allNode, minNode, dump, maxIter);
  return node;
}

CounterInversePagerankNode* NodeFactoryHelper::initCounterInversePagerankNode(unordered_map<string, string>* params) {
  long allNode;
  int maxIter;
  double dump;

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &allNode);
  sscanf((*params)["MAX_ITER"].c_str(), "%d", &maxIter);
  sscanf((*params)["DUMP"].c_str(), "%lf", &dump);


  CounterInversePagerankNode* node = new CounterInversePagerankNode(allNode, dump, maxIter);
  return node;
}

CounterInverseNode* NodeFactoryHelper::initCounterInverseNode(unordered_map<string, string>* params) {
  return new CounterInverseNode();
}

AlsNode* NodeFactoryHelper::initAlsNode(unordered_map<string, string>* params) {
  return new AlsNode(params);
}
