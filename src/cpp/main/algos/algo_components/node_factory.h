/*
 * node_factory.h
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#ifndef NODE_FACTORY_H_
#define NODE_FACTORY_H_

#include "../../common/util/cfg_reader.h"
#include "../../common/graph/filter_edge_list_builder.h"
#include "../../common/graph/crawl_edge_list_builder.h"
#include "node.h"
#include "../simrank_update/simrank_update_node.h"
#include "../simrank_store_first/simrank_store_first_node.h"
#include "../simrank_odd_even/simrank_odd_even_node.h"
#include "../pagerank/pagerank_node.h"
#include "../pagerank_non_block/pagerank_non_block_node.h"
#include "../clever_pagerank/clever_pagerank_node.h"
#include "../custom_non_block/custom_non_block_node.h"
#include "../custom_multi_nonblock/custom_multi_non_block_node.h"
#include "../psimrank/psimrank_node.h"
#include "../bitprop/bitprop_node.h"
#include "../counter_inverse/counter_inverse_node.h"
#include "../counter_inverse_pagerank/counter_inverse_pagerank_node.h"
#include "../als/als_node.h"

#include "../../common/util/util.h"
#include "../bitprop/estimation_handler.h"

#include <log4cpp/Category.hh>
#include "inode_factory.h"
#include "node_factory_helper.h"

class NodeFactory : public INodeFactory {
public:
	NodeFactory();
	void setPartitionConfigHandler(GraphPartitionConfigHandler*);
	Node* createNodeFromConfig(unordered_map<string, string>* params);
	std::vector<FailedEstimate>* readFailedEstimations(unordered_map<string, string>* params);
	EstimationHandler* createEstimationHandler(unordered_map<string, string>* params);
	unsigned char* initRandomVectorBits(long, int, double, int);
private:
	GraphPartitionConfigHandler* partConfigHandler;
	NodeFactoryHelper helper;
	EdgelistContainer* createEdgeListContainer(unordered_map<string, string>* params);
	EdgelistContainer* createEdgeListContainer(string, long, IEdgeListBuilder*);
	IEdgeListBuilder* createEdgeListBuilder(unordered_map<string, string>* params);
	SimrankStoreFirstNode* createSimrankStoreFirstNode(unordered_map<string, string>* params);
	SimrankOddEvenNode* createSimrankOddEvenNode(unordered_map<string, string>* params);
	PagerankNode* createPagerankNode(unordered_map<string, string>* params);
	PagerankNonBlockNode* createPagerankNonBlockNode(unordered_map<string, string>* params);
	PSimrankNode* createPSimrankNode(unordered_map<string, string>* params);
	BitpropNode* createBitpropNode(unordered_map<string, string>* params);
	CleverPagerankNode* createCleverPagerankNode(unordered_map<string, string>* params);
	CustomNonBlockNode* createCustomNonBlockNode(unordered_map<string, string>* params);
	CustomMultiNonBlockNode* createCustomMultiNonBlockNode(unordered_map<string, string>* params);
  CounterInverseNode* createCounterInverseNode(unordered_map<string, string>* params);
  CounterInversePagerankNode* createCounterInversePagerankNode(unordered_map<string, string>* params);
  AlsNode* createAlsNode(unordered_map<string, string>* params);
	log4cpp::Category* logger_;
	Util util;
};


#endif /* NODE_FACTORY_H_ */
