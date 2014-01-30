/*
 * node_task.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "algo_builder.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PatternLayout.hh"

void initLogger(unordered_map<string, string>* params) {
	string debugLevel;

	if (params->find(string("DEBUG_LEVEL")) != params->end()) {
		debugLevel = (*params)["DEBUG_LEVEL"];
	} else {
		debugLevel = string("INFO");
	}

	log4cpp::Category& root = log4cpp::Category::getRoot();

	if (debugLevel.compare("INFO") == 0) {
		root.setPriority(log4cpp::Priority::INFO);
	} else if (debugLevel.compare("DEBUG") == 0) {
		root.setPriority(log4cpp::Priority::DEBUG);
	} else if (debugLevel.compare("ERROR") == 0) {
		root.setPriority(log4cpp::Priority::ERROR);
	} else {
		root.setPriority(log4cpp::Priority::INFO);
	}

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

  initLogger(params);

  AlgoBuilder builder;
  Algo* algo = builder.buildFromConfig(params);

  if (algo->setUp()) {
    algo->run();
  }

  delete params;
}


