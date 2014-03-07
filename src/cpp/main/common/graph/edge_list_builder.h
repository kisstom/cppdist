/*
 * edge_list_builder.h
 *
 *  Created on: 2013.11.07.
 *      Author: kisstom
 */

#ifndef EDGE_LIST_BUILDER_H_
#define EDGE_LIST_BUILDER_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

#include "edgelist_container.h"
#include "../util/file_util.h"
#include "../util/util.h"
#include <log4cpp/Category.hh>
#include <sstream>
#include <algorithm>

using std::stringstream;

class EdgeListBuilder {
public:
	EdgeListBuilder();
	void buildFromFile(string fname);
	//EdgelistContainer* getContainer();
	void split(char* line, vector<long>& edges);
	void setContainer(EdgelistContainer* container);
private:
	EdgelistContainer* container_;
	Util util_;
	log4cpp::Category* logger_;
};


#endif /* EDGE_LIST_BUILDER_H_ */
