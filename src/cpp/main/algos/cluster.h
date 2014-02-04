/*
 * cluster.h
 *
 *  Created on: 2014.02.03.
 *      Author: kisstom
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include <tr1/unordered_map>
#include <string>
#include <vector>

#include "algo_builder.h"
#include "master.h"
#include "test_master_builder.h"
#include "../common/thread/main_thread.h"
#include "log4cpp/Category.hh"

using std::tr1::unordered_map;
using std::string;
using std::vector;

class Cluster {
public:
	Cluster(unordered_map<string, string>* params);
	void init();
	void start();
	Node* getNode(int);
	~Cluster();
private:
	void initMaster();
	void initNodes();
	void initNode(int nodeI);
	int numSlaves_;
	unordered_map<string, string>* params_;
	vector<AlgoBuilder*> builders_;
	Master* master_;
	log4cpp::Category* logger_;
};


#endif /* CLUSTER_H_ */
