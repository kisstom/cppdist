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

/*SimrankUpdateNode* NodeFactory::createSimrankUpdateNode(unordered_map<string, string>* params) {
  char fp_file[1024];
  char old_slavery_config_file[1024];
  char outputFileN[1024];
  int numPathes;

  strcpy(fp_file, (*params)["FP_FILE"].c_str());
  strcpy(old_slavery_config_file, (*params)["OLD_SLAVERY_CFG"].c_str());

  sscanf((*params)["NUM_PATHES"].c_str(), "%d", &numPathes);
  sprintf(outputFileN, "%sout_%s", (*params)["LOCAL_DIR"].c_str(), (*params)["SLAVE_INDEX"].c_str());
  FILE* outputFile = fopen(outputFileN, "w");
  if (outputFile == NULL) {
  	logger_->error("ERROR opening file %s", outputFileN);
  	return NULL;
  }

  SimrankUpdateNode* node = new SimrankUpdateNode(fp_file, old_slavery_config_file, numPathes, outputFile);
  return node;
}*/

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

  if (params->find("FILTER_NODE_FILE") == params->end()) {
    edgeListBuilder = new EdgeListBuilder;
  } else {
    FilterEdgeListBuilder* filterEdgeListBuilder = new FilterEdgeListBuilder;
    string nodesToDeleteFile = (*params)["FILTER_NODE_FILE"];
    filterEdgeListBuilder->readNodesToDelete(nodesToDeleteFile);
    edgeListBuilder = filterEdgeListBuilder;
  }

  char outputFileN[1024];
  sprintf(outputFileN, "%sout_%s", (*params)["LOCAL_DIR"].c_str(), (*params)["SLAVE_INDEX"].c_str());
  node->setEdgeListBuilder(edgeListBuilder);
  node->setOutputFile(string(outputFileN));
  node->setFingerPrintFile((*params)["FP_START_NAME"]);
  node->initData((*params)["INPUT_PARTITION"]);
  return node;
}
