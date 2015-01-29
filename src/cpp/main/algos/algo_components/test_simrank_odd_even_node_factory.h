/*
 * test_simrank_odd_even_node_factory.h
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#ifndef TEST_SIMRANK_ODD_EVEN_NODE_FACTORY_H_
#define TEST_SIMRANK_ODD_EVEN_NODE_FACTORY_H_

#include <vector>
#include <list>

#include "factories/old_partition_node_factory.h"
#include "node_factory_helper.h"
#include <log4cpp/Category.hh>
#include "factories/graph_partition_config_handler.h"
#include "../simple_mock_algo/simple_mock_node.h"

using std::vector;
using std::list;


class TestSimrankOddEvenNodeFactory : public OldPartitionNodeFactory {
public:
  TestSimrankOddEvenNodeFactory();
	Node* createNodeFromConfig(unordered_map<string, string>* params);
	void setEdgelistContainer(EdgelistContainer* container);
	void setFingerprints(vector<list<long*> >);
	void setPartConfHandler(GraphPartitionConfigHandler*);
private:
	Node* createSimrankOddEvenNode(unordered_map<string, string>* params);
	Node* createPSimrankNode(unordered_map<string, string>* params);
	Node* createSimpleMockNode(unordered_map<string, string>* params);
	EdgelistContainer* container_;
	GraphPartitionConfigHandler* partConfigHandler;
	vector<list<long*> > fprints_;
	log4cpp::Category* logger_;
};


#endif /* TEST_SIMRANK_ODD_EVEN_NODE_FACTORY_H_ */
