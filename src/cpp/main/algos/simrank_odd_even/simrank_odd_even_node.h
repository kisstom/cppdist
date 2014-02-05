/*
 * simrank_odd_even_node.h
 *
 *  Created on: 2014.01.13.
 *      Author: kisstom
 */

#ifndef SIMRANK_ODD_EVEN_NODE_H_
#define SIMRANK_ODD_EVEN_NODE_H_

#include <limits.h>

#include <tr1/unordered_map>
#include <list>

#include "../../common/graph/edgelist_container.h"
#include "../../common/random/random.h"
#include "../../common/random/hash_pseudo_random.h"
#include "../../common/components/mutex.h"
#include "../../common/graph/edge_list_builder.h"
#include "../node.h"
#include "../algo.h"

#include "random_generator_type.h"


using std::string;
using std::vector;
using std::tr1::unordered_map;

class SimrankOddEvenNode : public Node {
public:
	SimrankOddEvenNode();
	~SimrankOddEvenNode();
	SimrankOddEvenNode(short numFingerprints, short pathLen,
			int seed, GeneratorType type, long num_nodes, long min_node, long nextMinNode);
	void beforeIteration();
	bool afterIteration();
	void sender();
	void senderOdd();
	void senderEven();
	void initFromMaster(string);
	void initData(string);
	void final();

	long genEdge(long from);
	void serializeRequest(long, short);
	void serializeAnswer(long from, long to, short bufferIndex);
	bool incrementIndices();
	void incrementPathes();
	void initStartForAll(long from, long to, long numnodes,
			short numFingerPrints);
	void storeRequest(long from, short partIndex);
	void storeRequestedEdge(long from, long to);
	int hashToEdgeIndex(int hash, int length);

	void setFingerprints(vector<list<long*> >);
	void setMatrix(EdgelistContainer* matrix);
	void setNextNodes(unordered_map<long, long>);
	void setOutputFile(string fname);
	void setFingerPrintFile(string fpStartFname);
	void initFinishedPathes(vector<vector<long*> >);
	vector<list<long*> >* getPathes();
	vector<vector<long*> >* getFinishedPathes();
	bool* getOddIter() {return &oddIter_;}
	void initRandomGenerator(int, GeneratorType);
private:
	Mutex nextNodesMutex_;
	unordered_map<long, long> nextNodes_;
	unordered_map<long, vector<short> > sendBack_;

	vector<list<long*> > fingerprints_;
	vector<vector<long*> > finishedPathes_;

	short numFingerprints_;
	short pathLen_;
	short fpIndex_;
	short pathIndex_;
	bool oddIter_;
	long numNodes_;
	long minNode_;
	long nextMinNode_;

	string fpStartFname_;
	string outFileName_;
	// TODO ebbol mock-ot csinalni
	PseudoRandom* random_;
	EdgelistContainer* matrix_;

	log4cpp::Category* logger_;
};


#endif /* SIMRANK_ODD_EVEN_NODE_H_ */
