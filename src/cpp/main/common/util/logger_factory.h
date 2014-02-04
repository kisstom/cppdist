/*
 * logger_factory.h
 *
 *  Created on: 2014.02.04.
 *      Author: kisstom
 */

#ifndef LOGGER_FACTORY_H_
#define LOGGER_FACTORY_H_

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

#include <string>

using std::string;

class LoggerFactory {
public:
	static void initLogger(string level, string appender, string logfilename);
};


#endif /* LOGGER_FACTORY_H_ */
