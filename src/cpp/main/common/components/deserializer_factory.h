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
#include "../../algos/algo_components/node.h"

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
  Deserializer* createCustomMultiNonBlock(unordered_map<string, string>* params, Node* node);
  Deserializer* createCounterInverseDeserializer(unordered_map<string, string>* params, Node* node);
  Deserializer* createCounterInversePagerankDeserializer(unordered_map<string, string>* params, Node* node);
  Deserializer* createAls(unordered_map<string, string>* params, Node* node);
  Deserializer* createAlsMulti2Step(unordered_map<string, string>* params, Node* node);
	log4cpp::Category* logger_;
	Util util_;
};


#endif /* DESERIALIZER_FACTORY_H_ */
