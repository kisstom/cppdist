/*
 * simrank_store_first_node.h
 *
 *  Created on: 2013.11.06.
 *      Author: kisstom
 */

#ifndef SIMRANK_STORE_FIRST_NODE_H_
#define SIMRANK_STORE_FIRST_NODE_H_

#include "../node.h"
#include "../algo.h"
#include <tr1/unordered_map>
#include <vector>
#include "../../common/graph/edgelist_container.h"
#include "../../common/random/random_generator.h"
#include "../../common/components/mutex.h"
#include "../../common/graph/edge_list_builder.h"
#include <log4cpp/Category.hh>
#include <unistd.h>
#include <limits.h>
#include <list>

using std::tr1::unordered_map;
using std::vector;
using std::list;

class SimrankStoreFirstNode : public Node {
public:
	SimrankStoreFirstNode();
	~SimrankStoreFirstNode();
	SimrankStoreFirstNode(short numFingerprints, short pathLen,
			string fpStartFname, string outFileName);
	void beforeIteration();
	bool afterIteration();
	void sender();
	void initFromMaster(string);
	void initData(string, long, long);
	void final();

	void showSenderEnd(short);
	void showSenderEndForAll();
	void registerNodeFinished(short);
	long genEdge(long from);
	void serializeRequest(long, short);
	void serializeAnswer(long from, long to, short bufferIndex);
	void consumeRequest(long from, short partIndex);
	void consumeAnswer(long from, long to);
	bool incrementIndices();
	void senderFinishedDaemon();
	void initStartForAll(long from, long to, long numnodes,
			short numFingerPrints);
	void block();

	void setFingerprints(vector<list<long*> >);
	void setNextNodes(unordered_map<long, long>);
	void initFinishedPathes(vector<vector<long*> >);
	vector<list<long*> >* getPathes();
	vector<vector<long*> >* getFinishedPathes();
private:
	// nodes triggered to send back edge
	unordered_map<long, long> nextNodes_;

	// fingerprints
	vector<list<long*> > fingerprints_;
	vector<vector<long*> > finishedPathes_;
	short fpIndex_;
	short pathIndex_;
	short numFingerprints_;
	short pathLen_;
	short receiverFinished_;
	bool senderShouldStop_;

	string fpStartFname_;
	string outFileName_;
	// TODO ebbol mock-ot csinalni
	RandomGenerator* generator_;
	EdgelistContainer* matrix_;

	Mutex sendeBufferLock_;
	Mutex nextNodesMutex_;

	log4cpp::Category* logger_;
};


#endif /* SIMRANK_STORE_FIRST_NODE_H_ */
