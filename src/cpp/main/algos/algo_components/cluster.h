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
#include "master_builder.h"
#include "../../common/thread/main_thread.h"
#include "../../common/thread/setup_thread.h"
#include "log4cpp/Category.hh"

//using std::tr1::unordered_map;
using std::string;
using std::vector;

class Cluster {
public:
	Cluster(unordered_map<string, string>* params, vector<unordered_map<string, string> >* nodeParams,
			vector<INodeFactory*>, MasterBuilder*, vector<std::pair<string, string> >*, vector<long>*);
	void init();
	void start();
	void setUp();
	Node* getNode(int);
	AlgoBase* getAlgo(int);
	~Cluster();
private:
	void initMaster();
	void initNodes();
	void initNode(int nodeI);
	int numSlaves_;
	unordered_map<string, string>* params_;
	vector<unordered_map<string, string> >* nodeParams_;
	vector<std::pair<string, string> >* clusterNodeParams_;
	vector<AlgoBuilder*> builders_;
	MasterBase* master_;
	vector<INodeFactory*> nodeFactories_;
	MasterBuilder* masterBuilder_;
	vector<long>* partMinNodes;
	log4cpp::Category* logger_;
};


#endif /* CLUSTER_H_ */
