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
	} else {
    logger_->error("Unknown tpye of deserializer %s", nodeType.c_str());
	}

	logger_->info("Using deserializer type %s.", nodeType.c_str());
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

