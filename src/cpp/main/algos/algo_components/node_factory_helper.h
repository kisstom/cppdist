/*
 * node_factory_helper.h
 *
 *  Created on: 2014.02.05.
 *      Author: kisstom
 */

#ifndef NODE_FACTORY_HELPER_H_
#define NODE_FACTORY_HELPER_H_

#include <tr1/unordered_map>
#include "../simrank_odd_even/simrank_odd_even_node.h"
#include "../clever_pagerank/clever_pagerank_node.h"
#include "../pagerank/pagerank_node.h"
#include "../pagerank_non_block/pagerank_non_block_node.h"
#include "../psimrank/psimrank_node.h"
#include "../bitprop/bitprop_node.h"
#include "../custom_non_block/custom_non_block_node.h"
#include "../custom_multi_nonblock/custom_multi_non_block_node.h"
#include "../counter_inverse/counter_inverse_node.h"
#include "../counter_inverse_pagerank/counter_inverse_pagerank_node.h"
#include "../../common/util/util.h"
#include "inode_factory.h"

using std::tr1::unordered_map;
using std::string;

class NodeFactoryHelper {
public:
	NodeFactoryHelper();
	SimrankOddEvenNode* initSimrankOddEvenNode(unordered_map<string, string>* params);
	PSimrankNode* initPSimrankNode(unordered_map<string, string>* params);
	PagerankNode* initPagerankNode(unordered_map<string, string>* params);
	PagerankNonBlockNode* initPagerankNonBlockNode(unordered_map<string, string>* params);
	CleverPagerankNode* initCleverPagerankNode(unordered_map<string, string>* params);
	BitpropNode* initBitpropNode(unordered_map<string, string>* params);
	CustomNonBlockNode* initCustomNonBlockNode(unordered_map<string, string>* params);
	CustomMultiNonBlockNode* initCustomMultiNonBlockNode(unordered_map<string, string>* params);
	CounterInverseNode* initCounterInverseNode(unordered_map<string, string>* params);
	CounterInversePagerankNode* initCounterInversePagerankNode(unordered_map<string, string>* params);
private:
	log4cpp::Category* logger_;
	Util util;
};


#endif /* NODE_FACTORY_HELPER_H_ */
