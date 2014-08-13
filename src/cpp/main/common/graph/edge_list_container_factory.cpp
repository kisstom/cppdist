/*
 * edge_list_container_factory.cpp
 *
 *  Created on: 2014.08.12.
 *      Author: kisstom
 */

#include "edge_list_container_factory.h"


EdgeListContainerFactory::EdgeListContainerFactory() {
  logger_ = &log4cpp::Category::getInstance(std::string("EdgeListContainerFactory"));
}

IEdgeListBuilder* EdgeListContainerFactory::createEdgeListBuilder(unordered_map<string, string>* params) {
  IEdgeListBuilder* edgeListBuilder = NULL;

  if (params->find("USE_PREPROCESS") == params->end()) {
    edgeListBuilder = new EdgeListBuilder;
  } else {
    if ((*params)["USE_PREPROCESS"].compare("CRAWL") == 0) {
      long maxNodeToKeep;
      sscanf((*params)["MAX_NODE_TO_KEEP"].c_str(), "%ld", &maxNodeToKeep);
      CrawlEdgeListBuilder* crawlEdgeListBuilder = new CrawlEdgeListBuilder(
          maxNodeToKeep);
      edgeListBuilder = crawlEdgeListBuilder;
    } else if ((*params)["USE_PREPROCESS"].compare("FILTER") == 0) {
      FilterEdgeListBuilder* filterEdgeListBuilder = new FilterEdgeListBuilder;
      string nodesToDeleteFile = (*params)["FILTER_NODE_FILE"];
      filterEdgeListBuilder->readNodesToDelete(nodesToDeleteFile);
      edgeListBuilder = filterEdgeListBuilder;
    } else {
      logger_->error("ERROR. Unknown type of preprcessing %s.\n",
          (*params)["USE_PREPROCESS"].c_str());
      return NULL;
    }
  }

  return edgeListBuilder;
}

EdgelistContainer* EdgeListContainerFactory::createEdgeListContainer(unordered_map<string, string>* params) {
  util.checkParam(params, 2, "INPUT_PARTITION", "MIN_NODE");

  IEdgeListBuilder* builder = createEdgeListBuilder(params);
  long min_node;
  sscanf((*params)["MIN_NODE"].c_str(), "%ld", &min_node);
  logger_->info("Initing edge list container.");

  EdgelistContainer* matrix = new EdgelistContainer();
  matrix->initContainers();
  matrix->setMinnode(min_node);

  builder->setContainer(matrix);
  builder->buildFromFile((*params)["INPUT_PARTITION"]);

  logger_->info("Edge list container inited.");
  return matrix;
}

