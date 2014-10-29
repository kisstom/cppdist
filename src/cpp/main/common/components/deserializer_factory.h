/*
 * deserializer_factory.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef DESERIALIZER_FACTORY_H_
#define DESERIALIZER_FACTORY_H_

#include "../util/cfg_reader.h"
#include "../util/util.h"
#include "deserializer.h"
#include "../../algos/simrank_update/simrank_update_deserializer.h"
#include "../../algos/simrank_store_first/simrank_store_first_deserializer.h"
#include "../../algos/algo_components/node.h"
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

#include <log4cpp/Category.hh>

class DeserializerFactory {
public:
	DeserializerFactory();
	Deserializer* createDeserializerFromConfig(unordered_map<string, string>*, Node* node);
private:
	Deserializer* createSimrankUpdateDeserializer(unordered_map<string, string>*, Node* node);
	Deserializer* createSimrankStoreFirstDeserializer(unordered_map<string, string>*, Node* node);
	Deserializer* createSimrankOddEvenDeserializer(unordered_map<string, string>*, Node* node);
	Deserializer* createPagerankDeserializer(unordered_map<string, string>*, Node* node);
  Deserializer* createPagerankNonBlockDeserializer(unordered_map<string, string>*, Node* node);
	Deserializer* createPSimrankDeserializer(unordered_map<string, string>*, Node* node);
	Deserializer* createSimpleMockDeserializer(unordered_map<string, string>* params, Node* node);
	Deserializer* createBitpropDeserializer(unordered_map<string, string>* params, Node* node);
  Deserializer* createCleverPagerankDeserializer(unordered_map<string, string>*, Node* node);
  Deserializer* createCustomNonBlock(unordered_map<string, string>* params, Node* node);
  Deserializer* createCounterInverseDeserializer(unordered_map<string, string>* params, Node* node);
  Deserializer* createCounterInversePagerankDeserializer(unordered_map<string, string>* params, Node* node);
	log4cpp::Category* logger_;
	Util util_;
};


#endif /* DESERIALIZER_FACTORY_H_ */
