/*
 * logger_factory.cpp
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#include "logger_factory.h"

void LoggerFactory::initLogger(string debugLevel, string appenderType,
		string logfile_name = "") {
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
	log4cpp::Appender *appender;

	if (appenderType.compare("CONSOLE") == 0) {
		appender = new log4cpp::OstreamAppender("default", &std::cout);
	} else if (appenderType.compare("FILE") == 0) {
		appender = new log4cpp::FileAppender("default", logfile_name);
	} else {
		// TODO
	}

	log4cpp::PatternLayout * layout = new log4cpp::PatternLayout();
	layout->setConversionPattern("%d{%H:%M:%S,%l} %c %x: %m\n");
	appender->setLayout(layout);

	root.addAppender(appender);
}
