/*
 * edge_list_container_factory.h
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

#ifndef EDGE_LIST_CONTAINER_FACTORY_H_
#define EDGE_LIST_CONTAINER_FACTORY_H_

#include "../../common/graph/edgelist_container.h"
#include "../../common/graph/iedge_list_builder.h"
#include "../../common/graph/crawl_edge_list_builder.h"
#include "../../common/graph/filter_edge_list_builder.h"
#include "../../common/graph/edge_list_builder.h"
#include "../../common/util/util.h"

class EdgeListContainerFactory {
public:
  EdgeListContainerFactory();
  IEdgeListBuilder* createEdgeListBuilder(unordered_map<string, string>* params);
  EdgelistContainer* createEdgeListContainer(unordered_map<string, string>* params);
private:
  log4cpp::Category* logger_;
  Util util;
};


#endif /* EDGE_LIST_CONTAINER_FACTORY_H_ */
