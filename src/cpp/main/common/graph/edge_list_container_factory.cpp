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

EdgelistContainer* EdgeListContainerFactory::createEdgeListContainer(unordered_map<string, string>* params,
    long minNode, string fName) {
  IEdgeListBuilder* builder = createEdgeListBuilder(params);
  logger_->info("Initing edge list container.");

  EdgelistContainer* matrix = new EdgelistContainer();
  matrix->initContainers();
  matrix->setMinnode(minNode);

  builder->setContainer(matrix);
  builder->buildFromFile(fName);

  logger_->info("Edge list container inited.");
  return matrix;
}

