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

#include "inode_factory.h"
#include "node_factory_helper.h"

using std::vector;
using std::list;


class TestSimrankOddEvenNodeFactory : public INodeFactory {
public:
	Node* createNodeFromConfig(unordered_map<string, string>* params);
	void setEdgelistContainer(EdgelistContainer* container);
	void setFingerprints(vector<list<long*> >);
private:
	EdgelistContainer* container_;
	vector<list<long*> > fprints_;
};


#endif /* TEST_SIMRANK_ODD_EVEN_NODE_FACTORY_H_ */
