/*
 * crawl_edge_list_builder.h
 *
 *  Created on: 2014.03.11.
 *      Author: kisstom
 */

#ifndef CRAWL_EDGE_LIST_BUILDER_H_
#define CRAWL_EDGE_LIST_BUILDER_H_


#include <log4cpp/Category.hh>
//#include <sstream>
#include <algorithm>

#include "../util/file_util.h"
#include "../util/util.h"
#include "iedge_list_builder.h"

class CrawlEdgeListBuilder : public IEdgeListBuilder {
public:
  CrawlEdgeListBuilder(long);
  void buildFromFile(string fname);
private:
  log4cpp::Category* logger_;
  long maxNodeId_;

};


#endif /* CRAWL_EDGE_LIST_BUILDER_H_ */
