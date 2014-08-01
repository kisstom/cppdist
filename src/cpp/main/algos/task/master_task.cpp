/*
 * master_task.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "../algo_components/master_builder.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

#include "../../common/util/logger_factory.h"

void initLogger(unordered_map<string, string>* params) {
	string debugLevel, appender, logfileName;

	if (params->find(string("DEBUG_LEVEL")) != params->end()) {
		debugLevel = (*params)["DEBUG_LEVEL"];
	} else {
		debugLevel = string("INFO");
	}

	if (params->find(string("APPENDER")) != params->end()) {
	  appender = (*params)["APPENDER"];
	} else {
		appender = string("FILE");
	}

	logfileName = (*params)["MASTER_LOG"];
	printf("logf %s\n", logfileName.c_str());
	LoggerFactory::initLogger(debugLevel, appender, logfileName);

	log4cpp::Category* logger = &log4cpp::Category::getInstance(std::string("MasterTask"));
	logger->info("Logger started. Level %s.", debugLevel.c_str());

}

int main(int argc, char* argv[]) {
	CfgReader cfgreader;
  // should add checking
  cfgreader.read(argv[1]);
  unordered_map<string, string>* params = cfgreader.getParams();
  (*params)["NUM_SLAVES"] = string(argv[2]);

  initLogger(params);
  MasterBuilder builder;
  Master* master = builder.buildFromConfig(params);
  if (NULL == master) {
    fprintf(stderr, "Error creating master. Exiting.");
  }
  master->setUp();
  master->run();
}


