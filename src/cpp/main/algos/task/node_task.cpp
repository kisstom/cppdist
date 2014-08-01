/*
 * node_task.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "../algo_components/algo_builder.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PatternLayout.hh"

#include "../../common/util/logger_factory.h"
#include <string.h>

void initLogger(unordered_map<string, string>* params) {
	string debugLevel, appender, logfileName;

	string debugLev("DEBUG_LEVEL");
	if (params->find(debugLev) != params->end()) {
		debugLevel = (*params)["DEBUG_LEVEL"];
	} else {
		debugLevel = string("INFO");
	}

	if (params->find(string("APPENDER")) != params->end()) {
	  appender = (*params)["APPENDER"];
	} else {
		appender = string("FILE");
	}

	logfileName = (*params)["LOCAL_DIR"];
	logfileName.append("/log_");
	logfileName.append((*params)["SLAVE_INDEX"]);

	LoggerFactory::initLogger(debugLevel, appender, logfileName);

	log4cpp::Category* logger = &log4cpp::Category::getInstance(std::string("NodeTask"));
	logger->info("Logger started. Level %s.", debugLevel.c_str());
}

int main(int argc, char* argv[]) {
	CfgReader cfgreader;
  // should add checking
  cfgreader.read(argv[1]);
  unordered_map<string, string>* params = cfgreader.getParams();
  (*params)["SLAVE_INDEX"] = string(argv[2]);
  (*params)["NUM_SLAVES"] = string(argv[3]);

  (*params)["INPUT_PARTITION"] = string(argv[4]);
  (*params)["NUM_NODES"] = string(argv[5]);
  (*params)["MIN_NODE"] = string(argv[6]);
  (*params)["NEXT_MIN_NODE"] = string(argv[7]);

  initLogger(params);

  AlgoBuilder builder;
  INodeFactory* nodeFactory = new NodeFactory;
  builder.setNodeFactory(nodeFactory);

  Algo* algo = builder.buildFromConfig(params);

  if (algo->setUp()) {
    algo->run();
  }

  algo->final();
  delete params;
}


