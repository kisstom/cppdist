/*
 * filter_edge_list_builder.h
 *
 *  Created on: 2014.03.07.
 *      Author: kisstom
 */

#ifndef FILTER_EDGE_LIST_BUILDER_H_
#define FILTER_EDGE_LIST_BUILDER_H_

#include <fstream>
#include <set>
#include <algorithm>
#include <cstdio>

#include "IEdgeListBuilder.h"
#include "../util/util.h"


using std::fstream;
using std::set;

class FilterEdgeListBuilder : public IEdgeListBuilder {
public:
    FilterEdgeListBuilder();
    void buildFromFile(string fname);
private:
    inline bool isDeletedNode(long node) {
      return nodesShouldDelete_.find(node) == nodesShouldDelete_.end();
    }
    void readNodesToDelete(string fname);
    EdgelistContainer* container_;
    log4cpp::Category* logger_;
    set<long> nodesShouldDelete_;
};


#endif /* FILTER_EDGE_LIST_BUILDER_H_ */
