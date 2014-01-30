/*
 * master_task.cpp
 *
 *  Created on: 2013.08.15.
 *      Author: kisstom
 */

#include "master_builder.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

int main(int argc, char* argv[]) {
	log4cpp::Appender *appender = new log4cpp::OstreamAppender("console", &std::cout);
	appender->setLayout(new log4cpp::BasicLayout());

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(appender);

	CfgReader cfgreader;
  // should add checking
  cfgreader.read(argv[1]);
  unordered_map<string, string>* params = cfgreader.getParams();
  (*params)["NUM_SLAVES"] = string(argv[2]);

  string debugLevel("INFO");
  if (params->find(string("DEBUG_LEVEL")) != params->end()) {
  	debugLevel = (*params)[string("DEBUG_LEVEL")];
  }

  if (debugLevel.compare("INFO") == 0) {
		root.setPriority(log4cpp::Priority::INFO);
	} else if (debugLevel.compare("DEBUG") == 0) {
		root.setPriority(log4cpp::Priority::DEBUG);
	} else if (debugLevel.compare("ERROR") == 0) {
		root.setPriority(log4cpp::Priority::ERROR);
	} else {
		root.setPriority(log4cpp::Priority::INFO);
	}


  MasterBuilder builder;
  Master* master = builder.buildFromConfig(params);

  master->setUp();
  master->run();
}


