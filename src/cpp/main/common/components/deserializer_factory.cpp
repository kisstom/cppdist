/*
 * deserializer_factory.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "deserializer_factory.h"

DeserializerFactory::DeserializerFactory() {
	logger_ = &log4cpp::Category::getInstance(std::string("DeserializerFactory"));
}

Deserializer* DeserializerFactory::createDeserializerFromConfig(unordered_map<string, string>* params, Node* node) {
	string nodeType = (*params)["DESERIALIZER_TYPE"];
	Deserializer* deserializer = NULL;
	if (nodeType.compare("SIMRANK_UPDATE") == 0) {
		deserializer = createSimrankUpdateDeserializer(params, node);
	} else if (nodeType.compare("SIMRANK_STORE_FIRST") == 0) {
		deserializer = createSimrankStoreFirstDeserializer(params, node);
	} else if (nodeType.compare("SIMRANK_ODD_EVEN") == 0) {
		deserializer = createSimrankOddEvenDeserializer(params, node);
	} else if (nodeType.compare("PAGERANK") == 0) {
	  deserializer = createPagerankDeserializer(params, node);
	} else if (nodeType.compare("PSIMRANK") == 0) {
    deserializer = createPSimrankDeserializer(params, node);
  } else if (nodeType.compare("SIMPLE_MOCK") == 0) {
    deserializer = createSimpleMockDeserializer(params, node);
  } else if (nodeType.compare("BITPROP") == 0) {
    deserializer = createBitpropDeserializer(params, node);
  } else if (nodeType.compare("CLEVER_PAGERANK") == 0) {
    deserializer = createCleverPagerankDeserializer(params, node);
  } else {
    logger_->error("Unknown tpye of deserializer %s", nodeType.c_str());
	}

	logger_->info("Using deserializer type %s.", nodeType.c_str());
	return deserializer;
}

Deserializer* DeserializerFactory::createSimpleMockDeserializer(unordered_map<string, string>* params, Node* node) {
  SimpleMockNode* simpleMockNode = static_cast<SimpleMockNode*>(node);
  SimpleMockDeserializer* deserializer = new SimpleMockDeserializer;
  deserializer->setNode(simpleMockNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createSimrankStoreFirstDeserializer(unordered_map<string, string>* params, Node* node) {
  SimrankStoreFirstNode* simrankStoreFirstNode = static_cast<SimrankStoreFirstNode*>(node);
  SimrankStoreFirstDeserializer* deserializer = new SimrankStoreFirstDeserializer;
  deserializer->setNode(simrankStoreFirstNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createSimrankUpdateDeserializer(unordered_map<string, string>* params, Node* node) {
  SimrankUpdateNode* simrankUpdateNode = static_cast<SimrankUpdateNode*>(node);
  SimrankUpdateDeserializer* deserializer = new SimrankUpdateDeserializer;
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createSimrankOddEvenDeserializer(unordered_map<string, string>* params, Node* node) {
  SimrankOddEvenNode* simrankUpdateNode = static_cast<SimrankOddEvenNode*>(node);
  SimrankOddEvenDeserializer* deserializer = new SimrankOddEvenDeserializer;
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createPagerankDeserializer(unordered_map<string, string>*, Node* node) {
  PagerankNode* pagerankNodeNode = static_cast<PagerankNode*>(node);
  PagerankDeserializer* deserializer = new PagerankDeserializer;
  deserializer->setNode(pagerankNodeNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createPSimrankDeserializer(unordered_map<string, string>* params, Node* node) {
  PSimrankNode* simrankUpdateNode = static_cast<PSimrankNode*>(node);
  PSimrankDeserializer* deserializer = new PSimrankDeserializer;
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createCleverPagerankDeserializer(unordered_map<string, string>* params, Node* node) {
  CleverPagerankNode* simrankUpdateNode = static_cast<CleverPagerankNode*>(node);
  CleverPagerankDeserializer* deserializer = new CleverPagerankDeserializer;
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createBitpropDeserializer(unordered_map<string, string>* params, Node* node) {
  util_.checkParam(params, 1, "NUM_CODING_BYTES");
  short numCodingBytes = -1;
  sscanf((*params)["NUM_CODING_BYTES"].c_str(), "%hd", &numCodingBytes);

  BitpropNode* simrankUpdateNode = static_cast<BitpropNode*>(node);
  BitpropDeserializer* deserializer = new BitpropDeserializer(numCodingBytes);
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

