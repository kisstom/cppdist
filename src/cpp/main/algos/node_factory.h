/*
 * node_factory.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef NODE_FACTORY_H_
#define NODE_FACTORY_H_

#include "../common/util/cfg_reader.h"
#include "node.h"
#include "simrank_update/simrank_update_node.h"
#include "simrank_store_first/simrank_store_first_node.h"
#include "simrank_odd_even/simrank_odd_even_node.h"
#include <log4cpp/Category.hh>

class NodeFactory {
public:
	NodeFactory();
	Node* createNodeFromConfig(unordered_map<string, string>* params);
private:
	SimrankStoreFirstNode* createSimrankStoreFirstNode(unordered_map<string, string>* params);
	SimrankOddEvenNode* createSimrankOddEvenNode(unordered_map<string, string>* params);
	//SimrankUpdateNode* createSimrankUpdateNode(unordered_map<string, string>* params);
	log4cpp::Category* logger_;
};


#endif /* NODE_FACTORY_H_ */
