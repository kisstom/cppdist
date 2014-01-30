/*
 * edgelist_container.h
 *
 *  Created on: 2013.07.30.
 *      Author: kisstom
 */

#ifndef EDGELIST_CONTAINER_H_
#define EDGELIST_CONTAINER_H_

#include <vector>
#include "../util/util.h"
using std::vector;
#include "log4cpp/Category.hh"

class EdgelistContainer {
public:
	EdgelistContainer();
	~EdgelistContainer();
	void setStartEdges(vector<long>*);
	void setEdgeList(vector<long>*);
	void addEdge(long nodeId, long edge);
	void setMinnode(long);
	bool shouldDeleteContainers();
	void initContainers();
	void setFinish();
	long neighborhoodSize(long nodeId);
  bool containsEdge(long, long);
  bool operator==(EdgelistContainer& rhs)const;
  void flush(FILE* f);

  long getEdgeAtPos(long, int);
	long getMinnode() const;
	long getNumberOfNodes() const;
	long getNumberOfEdges() const;
private:
	vector<long>* start_edges_;
	vector<long>* edge_list_;
	long minnode_;
	bool shouldDeleteContainers_;
	log4cpp::Category* logger_;
};


#endif /* EDGELIST_CONTAINER_H_ */
