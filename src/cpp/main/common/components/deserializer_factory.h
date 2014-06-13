/*
 * deserializer_factory.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef DESERIALIZER_FACTORY_H_
#define DESERIALIZER_FACTORY_H_

#include "../util/cfg_reader.h"
#include "deserializer.h"
#include "../../algos/simrank_update/simrank_update_deserializer.h"
#include "../../algos/simrank_store_first/simrank_store_first_deserializer.h"
#include "../../algos/node.h"
#include "../../algos/simrank_update/simrank_update_node.h"
#include "../../algos/simrank_store_first/simrank_store_first_node.h"
#include "../../algos/simrank_odd_even/simrank_odd_even_deserializer.h"
#include "../../algos/simrank_odd_even/simrank_odd_even_node.h"
#include "../../algos/pagerank/pagerank_deserializer.h"
#include "../../algos/pagerank/pagerank_node.h"
#include "../../algos/psimrank/psimrank_node.h"
#include "../../algos/psimrank/psimrank_deserializer.h"
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
	Deserializer* createPSimrankDeserializer(unordered_map<string, string>*, Node* node);
	log4cpp::Category* logger_;
};


#endif /* DESERIALIZER_FACTORY_H_ */
