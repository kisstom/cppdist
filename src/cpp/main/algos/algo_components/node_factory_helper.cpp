#include "node_factory_helper.h"


NodeFactoryHelper::NodeFactoryHelper() {
	logger_ = &log4cpp::Category::getInstance(std::string("NodeFactoryHelper"));
}

BitpropNode* NodeFactoryHelper::initBitpropNode(unordered_map<string, string>* params) {
  util.checkParam(params, 4, "NUM_CODING_BYTES", "EPSILON", "EST_INDEX", "MIN_NODE");
  int numCodingBytes;
  double epsilon;
  int estIndex;
  long min_node;

  sscanf((*params)["NUM_CODING_BYTES"].c_str(), "%d", &numCodingBytes);
  sscanf((*params)["EPSILON"].c_str(), "%lf", &epsilon);
  sscanf((*params)["EST_INDEX"].c_str(), "%d", &estIndex);
  sscanf((*params)["MIN_NODE"].c_str(), "%ld", &min_node);

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

SimrankOddEvenNode* NodeFactoryHelper::initSimrankOddEvenNode(unordered_map<string, string>* params) {
	GeneratorType type = SRAND;
	int seed;
	short numPathes;
	short pathLen;
	long num_nodes, min_node, next_min_node;
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

	sscanf((*params)["NUM_NODES"].c_str(), "%ld", &num_nodes);
	sscanf((*params)["MIN_NODE"].c_str(), "%ld", &min_node);
	sscanf((*params)["NEXT_MIN_NODE"].c_str(), "%ld", &next_min_node);

	SimrankOddEvenNode* node = new SimrankOddEvenNode(numPathes, pathLen, seed,
			type, num_nodes, min_node, next_min_node);
	return node;
}

PSimrankNode* NodeFactoryHelper::initPSimrankNode(unordered_map<string, string>* params) {
  GeneratorType type = SRAND;
  int seed;
  short numPathes;
  short pathLen;
  long num_nodes, min_node, next_min_node;
  sscanf((*params)["NUM_PATHES"].c_str(), "%hd", &numPathes);
  sscanf((*params)["PATH_LEN"].c_str(), "%hd", &pathLen);

  sscanf((*params)["NUM_NODES"].c_str(), "%ld", &num_nodes);
  sscanf((*params)["MIN_NODE"].c_str(), "%ld", &min_node);
  sscanf((*params)["NEXT_MIN_NODE"].c_str(), "%ld", &next_min_node);

  PSimrankNode* node = new PSimrankNode(numPathes, pathLen, num_nodes, min_node, next_min_node);
  return node;
}

CleverPagerankNode* NodeFactoryHelper::initCleverPagerankNode(unordered_map<string, string>* params) {
  long allNode, minNode;
  int maxIter;
  double dump;

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &allNode);
  sscanf((*params)["MAX_ITER"].c_str(), "%d", &maxIter);
  sscanf((*params)["DUMP"].c_str(), "%lf", &dump);
  sscanf((*params)["MIN_NODE"].c_str(), "%ld", &minNode);

  CleverPagerankNode* node = new CleverPagerankNode(allNode, minNode, dump, maxIter);
  return node;
}

CustomNonBlockNode* NodeFactoryHelper::initCustomNonBlockNode(unordered_map<string, string>* params) {
  long allNode, minNode;
  int maxIter;
  double dump;

  sscanf((*params)["NUMLINE"].c_str(), "%ld", &allNode);
  sscanf((*params)["MAX_ITER"].c_str(), "%d", &maxIter);
  sscanf((*params)["DUMP"].c_str(), "%lf", &dump);
  sscanf((*params)["MIN_NODE"].c_str(), "%ld", &minNode);

  CustomNonBlockNode* node = new CustomNonBlockNode(allNode, minNode, dump, maxIter);
  return node;
}
