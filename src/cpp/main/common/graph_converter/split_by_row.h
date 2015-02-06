/*
 * split_by_row.h
 *
 *  Created on: 2013.12.11.
 *      Author: kisstom
 */

#ifndef SPLIT_BY_ROW_H_
#define SPLIT_BY_ROW_H_

#include <string>
#include <string.h>
#include <sstream>

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PatternLayout.hh"

using std::string;

class SplitByRow {
public:
	SplitByRow(string, string, char*, long, string);
	SplitByRow();
	~SplitByRow();
	void run();
	bool init();
	long countEdges(char* line);
private:
	void addNextPartToCfg(long, long);
	bool openNextPartition();
	bool initPartition();
	bool openSlaveryFile();
	FILE* partition_;
	FILE* slavery_;
	FILE* input_;

	string inputFile_;
	char outputFilePrefix_[1024];
	long numNodesPerPart_;
	const int ROWLEN;
	log4cpp::Category* logger_;
	long numRow_;
	int partIndex_;
	string slaverFile_;
	long minnode_;
	long numNodes_;
};


#endif /* SPLIT_BY_ROW_H_ */
