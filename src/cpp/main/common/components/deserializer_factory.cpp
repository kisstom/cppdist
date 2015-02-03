/*
 * deserializer_factory.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "deserializer_factory.h"
#include "../../algos/simrank_update/simrank_update_deserializer.h"
#include "../../algos/simrank_store_first/simrank_store_first_deserializer.h"
#include "../../algos/simrank_update/simrank_update_node.h"
#include "../../algos/simrank_store_first/simrank_store_first_node.h"
#include "../../algos/simrank_odd_even/simrank_odd_even_deserializer.h"
#include "../../algos/simrank_odd_even/simrank_odd_even_node.h"
#include "../../algos/pagerank/pagerank_deserializer.h"
#include "../../algos/pagerank/pagerank_node.h"
#include "../../algos/pagerank_non_block/pagerank_non_block_node.h"
#include "../../algos/pagerank_non_block/pagerank_non_block_deserializer.h"
#include "../../algos/clever_pagerank/clever_pagerank_deserializer.h"
#include "../../algos/clever_pagerank/clever_pagerank_node.h"
#include "../../algos/custom_non_block/custom_non_block_deserializer.h"
#include "../../algos/custom_non_block/custom_non_block_node.h"
#include "../../algos/custom_multi_nonblock/custom_multi_non_block_deserializer.h"
#include "../../algos/custom_multi_nonblock/custom_multi_non_block_node.h"
#include "../../algos/counter_inverse/counter_inverse_node.h"
#include "../../algos/counter_inverse/counter_inverse_deserializer.h"
#include "../../algos/counter_inverse_pagerank/counter_inverse_pagerank_node.h"
#include "../../algos/counter_inverse_pagerank/counter_inverse_pagerank_deserializer.h"
#include "../../algos/psimrank/psimrank_node.h"
#include "../../algos/psimrank/psimrank_deserializer.h"
#include "../../algos/simple_mock_algo/simple_mock_deserializer.h"
#include "../../algos/simple_mock_algo/simple_mock_node.h"
#include "../../algos/bitprop/bitprop_deserializer.h"
#include "../../algos/bitprop/bitprop_node.h"
#include "../../algos/als/als_deserializer.h"
#include "../../algos/als/als_node.h"
#include "../../algos/als_multi_2step/als_multi_2step_deserializer.h"
#include "../../algos/als_multi_2step/als_multi_2step.h"

DeserializerFactory::DeserializerFactory() {
	logger_ = &log4cpp::Category::getInstance(std::string("DeserializerFactory"));
}

Deserializer* DeserializerFactory::createDeserializerFromConfig(unordered_map<string, string>* params, Node* node) {
  string nodeType = (*params)["DESERIALIZER_TYPE"];
  logger_->info("Creating deserializer %s.", nodeType.c_str());

	Deserializer* deserializer = NULL;
	if (nodeType.compare("SIMRANK_UPDATE") == 0) {
		deserializer = createSimrankUpdateDeserializer(params, node);
	} else if (nodeType.compare("SIMRANK_STORE_FIRST") == 0) {
		deserializer = createSimrankStoreFirstDeserializer(params, node);
	} else if (nodeType.compare("SIMRANK_ODD_EVEN") == 0) {
		deserializer = createSimrankOddEvenDeserializer(params, node);
	} else if (nodeType.compare("PAGERANK") == 0) {
	  deserializer = createPagerankDeserializer(params, node);
	} else if (nodeType.compare("PAGERANK_NON_BLOCK") == 0) {
    deserializer = createPagerankNonBlockDeserializer(params, node);
  } else if (nodeType.compare("PSIMRANK") == 0) {
    deserializer = createPSimrankDeserializer(params, node);
  } else if (nodeType.compare("SIMPLE_MOCK") == 0) {
    deserializer = createSimpleMockDeserializer(params, node);
  } else if (nodeType.compare("BITPROP") == 0) {
    deserializer = createBitpropDeserializer(params, node);
  } else if (nodeType.compare("CLEVER_PAGERANK") == 0) {
    deserializer = createCleverPagerankDeserializer(params, node);
  } else if (nodeType.compare("CUSTOM_NON_BLOCK") == 0) {
    deserializer = createCustomNonBlock(params, node);
  } else if (nodeType.compare("COUNTER_INVERSE") == 0) {
    deserializer = createCounterInverseDeserializer(params, node);
  } else if (nodeType.compare("COUNTER_INVERSE_PAGERANK") == 0) {
    deserializer = createCounterInversePagerankDeserializer(params, node);
  } else if (nodeType.compare("CUSTOM_MULTI_NONBLOCK") == 0) {
    deserializer = createCustomMultiNonBlock(params, node);
  } else if (nodeType.compare("ALS") == 0 || nodeType.compare("ALS_MULTI") == 0) {
    deserializer = createAls(params, node);
  } else if (nodeType.compare("ALS_MULTI_2STEP") == 0) {
    deserializer = createAlsMulti2Step(params, node);
  } else {
    logger_->error("Unknown tpye of deserializer %s", nodeType.c_str());
	}

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

Deserializer* DeserializerFactory::createPagerankNonBlockDeserializer(unordered_map<string, string>*, Node* node) {
  PagerankNonBlockNode* pagerankNodeNode = static_cast<PagerankNonBlockNode*>(node);
  PagerankNonBlockDeserializer* deserializer = new PagerankNonBlockDeserializer;
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

Deserializer* DeserializerFactory::createCustomNonBlock(unordered_map<string, string>* params, Node* node) {
  CustomNonBlockNode* simrankUpdateNode = static_cast<CustomNonBlockNode*>(node);
  CustomNonBlockDeserializer* deserializer = new CustomNonBlockDeserializer;
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createCustomMultiNonBlock(unordered_map<string, string>* params, Node* node) {
  CustomMultiNonBlockNode* simrankUpdateNode = static_cast<CustomMultiNonBlockNode*>(node);
  CustomMultiNonBlockDeserializer* deserializer = new CustomMultiNonBlockDeserializer;
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createCounterInverseDeserializer(unordered_map<string, string>* params, Node* node) {
  CounterInverseNode* simrankUpdateNode = static_cast<CounterInverseNode*>(node);
  CounterInverseDeserializer* deserializer = new CounterInverseDeserializer;
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::
createCounterInversePagerankDeserializer(unordered_map<string, string>* params, Node* node) {
  CounterInversePagerankNode* simrankUpdateNode = static_cast<CounterInversePagerankNode*>(node);
  CounterInversePagerankDeserializer* deserializer = new CounterInversePagerankDeserializer;
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createAls(unordered_map<string, string>* params, Node* node) {
  AlsNode* simrankUpdateNode = static_cast<AlsNode*>(node);
  int featSize;
  sscanf((*params)["NUM_FEAT"].c_str(), "%d", &featSize);

  AlsDeserializer* deserializer = new AlsDeserializer(featSize);
  deserializer->setNode(simrankUpdateNode);
  return deserializer;
}

Deserializer* DeserializerFactory::createAlsMulti2Step(unordered_map<string, string>* params, Node* node) {
  AlsMulti2Step* simrankUpdateNode = static_cast<AlsMulti2Step*>(node);
  int featSize;
  sscanf((*params)["NUM_FEAT"].c_str(), "%d", &featSize);

  AlsMulti2StepDeserializer* deserializer = new AlsMulti2StepDeserializer(featSize);
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

