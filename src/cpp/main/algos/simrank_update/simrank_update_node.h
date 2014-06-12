/*
 * node.h
 *
 *  Created on: 2013.07.30.
 *      Author: kisstom
 */

#ifndef SIMRANK_UPDATE_NODE_H_
#define SIMRANK_UPDATE_NODE_H_

#include <list>
#include "../../common/graph/sorted_vector_node_matrix.h"
#include "../../common/random/random_generator.h"
#include "../../common/old/random_walk.h"
#include "../node.h"
//#include "simrank_update_deserializer.h"
#include "../../common/components/mutex.h"
#include "../algo.h"
#include "../../common/components/fingerprint_reader.h"

#include "log4cpp/Category.hh"

using std::list;

class SimrankUpdateNode : public Node {
public:
	SimrankUpdateNode();
	SimrankUpdateNode(char* fpFile, char* slaveryConfigFile, int numPathes, FILE* outputFile);
	long changeOrPreserve(long from, long to);
	long generateRandomNeighbour(long, EdgelistContainer*);
	long generateRandomNeighbour(long, EdgelistContainer*,
			EdgelistContainer*);
	void serializeToSender(const RandomWalk&, int bufferIndex);
  long nextEdge(RandomWalk*);
  long genEdge(RandomWalk*);
  virtual void update(RandomWalk*);
  void incrementRandomWalkSize(RandomWalk* rw, long edge);
  void finishPath(RandomWalk*);

  void beforeIteration(string msg);
  bool afterIteration();
  void sender();
  void initFromMaster(string);
  void initData(string, long, long);
  void final();

	void setSeed(int);
	void setFpIndex(short);
	void setEdgeInPathes(short);
	void setMatrix(SortedVectorNodeMatrix*);
	bool flipCoinToStay(long);
	void printFp(RandomWalk*);
	~SimrankUpdateNode();
private:
	SortedVectorNodeMatrix* matrix_;
	//SenderBuffer* senderBuffer_;
	FingerprintReader* fpReader_;
	RandomGenerator* randomGenerator_;

	list<RandomWalk>* currentFp_;
	vector<RandomWalk> finishedFp_;
  vector<RandomWalk> tmpFp_;
  tr1::unordered_map<long, long> nextEdgeMap_;
	//tr1::unordered_map<long, long> rand_outlinks_;

  char fpFileName_[1024];
  char slaveryConfigFileName_[1024];
	//int partIndex_;
	int numPathes_;
	short edgeInPath_;
	short fpIndex_;
	Mutex threadLock_;
	Mutex genEdgethreadLock_;
	//Algo* algo_;
	log4cpp::Category* logger_;
	FILE* output_file_;
};


#endif /* NODE_H_ */
