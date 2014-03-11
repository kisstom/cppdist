/*
 * node_factory.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "node_factory.h"

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
  IEdgeListBuilder* edgeListBuilder;

  if (params->find("USE_PREPROCESS") == params->end()) {
    edgeListBuilder = new EdgeListBuilder;
  } else {
    if ((*params)["USE_PREPROCESS"].compare("CRAWL") == 0) {
      long maxNodeToKeep;
      sscanf((*params)["MAX_NODE_TO_KEEP"].c_str(), "%ld", &maxNodeToKeep);
      CrawlEdgeListBuilder*  crawlEdgeListBuilder = new CrawlEdgeListBuilder(maxNodeToKeep);
      edgeListBuilder = crawlEdgeListBuilder;
    } else if ((*params)["USE_PREPROCESS"].compare("FILTER") == 0) {
      FilterEdgeListBuilder* filterEdgeListBuilder = new FilterEdgeListBuilder;
      string nodesToDeleteFile = (*params)["FILTER_NODE_FILE"];
      filterEdgeListBuilder->readNodesToDelete(nodesToDeleteFile);
      edgeListBuilder = filterEdgeListBuilder;
    } else {
      logger_->error("ERROR. Unknown type of preprcessing %s.\n", (*params)["USE_PREPROCESS"].c_str());
      return NULL;
    }
  }

  char outputFileN[1024];
  sprintf(outputFileN, "%sout_%s", (*params)["LOCAL_DIR"].c_str(), (*params)["SLAVE_INDEX"].c_str());
  node->setEdgeListBuilder(edgeListBuilder);
  node->setOutputFile(string(outputFileN));
  node->setFingerPrintFile((*params)["FP_START_NAME"]);
  node->initData((*params)["INPUT_PARTITION"]);
  return node;
}
