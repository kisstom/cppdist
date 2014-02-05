/*
 * node_factory_helper.h
 *
 *  Created on: 2014.02.05.
 *      Author: kisstom
 */

#ifndef NODE_FACTORY_HELPER_H_
#define NODE_FACTORY_HELPER_H_

#include <tr1/unordered_map>
#include "simrank_odd_even/simrank_odd_even_node.h"
#include "inode_factory.h"

using std::tr1::unordered_map;
using std::string;

class NodeFactoryHelper {
public:
	NodeFactoryHelper();
	SimrankOddEvenNode* initSimrankOddEvenNode(unordered_map<string, string>* params);
private:
	log4cpp::Category* logger_;
};


#endif /* NODE_FACTORY_HELPER_H_ */
