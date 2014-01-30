/*
 * split_by_row.cpp
 *
 *  Created on: 2013.12.11.
 *      Author: kisstom
 */
#include "split_by_row.h"

using std::stringstream;

SplitByRow::SplitByRow() : ROWLEN(15000000) {}

SplitByRow::SplitByRow(string inputFile, string logfile, char* outputPrefix, long numNodesPerPart,
		string slaverFile, int initSlavePort) : ROWLEN(15000000) {
	inputFile_ = inputFile;
	strcpy(outputFilePrefix_, outputPrefix);

	numNodes_ = 0;
	minnode_ = 0;
	numRow_ = 0;
	partIndex_ = 0;
	numNodesPerPart_ = numNodesPerPart;
	partition_ = NULL;
	slavery_ = NULL;
	input_ = NULL;
	initSlavePort_ = initSlavePort;
	slavePort_ = initSlavePort;
	slaverFile_ = slaverFile;

	log4cpp::Appender *appender = new log4cpp::FileAppender("default", logfile);
	log4cpp::PatternLayout * layout = new log4cpp::PatternLayout();
	layout->setConversionPattern("%d{%H:%M:%S,%l} %c %x: %m\n");
	appender->setLayout(layout);

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.addAppender(appender);
	logger_ = &log4cpp::Category::getInstance(std::string("SplitByRow"));
	logger_->info("Started.");
}

SplitByRow::~SplitByRow() {
  if (partition_) fclose(partition_);
  if (slavery_) fclose(slavery_);
  if (input_) fclose(input_);
}

bool SplitByRow::init() {
	logger_->info("Init.");
	input_ = fopen(inputFile_.c_str(), "r");
	if (input_ == NULL) {
		logger_->error("Error opening file %s.", inputFile_.c_str());
		return false;
	}

	if (!initPartition()) {
		return false;
	}

	if (!openSlaveryFile()) {
		return false;
	}

	return true;
}

void SplitByRow::addNextPartToCfg(long numnode, long numedge) {
	fprintf(slavery_, "%d %s_%d.txt %ld %ld %ld\n",
	         slavePort_, outputFilePrefix_, partIndex_, numnode, numedge, minnode_);

}

void SplitByRow::run() {
	logger_->info("Starting run.");
	char* line = new char[ROWLEN];
	long numedge = 0;
  while (fgets(line, ROWLEN, input_)) {
    ++numRow_;
    ++numNodes_;
    if (numRow_ > numNodesPerPart_) {
    	addNextPartToCfg(numRow_ - 1, numedge);
    	numRow_ = 1;
    	numedge = 0;
    	if (!openNextPartition()) {
    		delete[] line;
    		return;
    	}
    }

    fprintf(partition_, "%s", line);
    numedge += countEdges(line);
  }

  addNextPartToCfg(numRow_, numedge);

  delete[] line;
  logger_->info("Run finished.");
}

long SplitByRow::countEdges(char* line) {
	int len = strlen(line);
	if (len > 0 && line[len - 1] == '\n') {
		line[len - 1] = '\0';
	}

	long count = 0;
	stringstream ss(stringstream::in | stringstream::out);
	ss << line;
	long edge;

	while (ss.peek() != EOF) {
		++count;
	  ss >> edge;
	}
	return count;
}

bool SplitByRow::openSlaveryFile() {
	slavery_ = fopen(slaverFile_.c_str(), "w");
	if (slavery_ == NULL) {
	  logger_->error("Error opening file %s.", slaverFile_.c_str());
	  return false;
	}
	return true;
}

bool SplitByRow::initPartition() {
	char partitionName[1024];
	sprintf(partitionName, "%s_%d.txt", outputFilePrefix_, partIndex_);
	partition_ = fopen(partitionName, "w");
	if (NULL == partition_) {
	  logger_->error("Error opening file %s.", partitionName);
	  return false;
	}

	return true;
}

bool SplitByRow::openNextPartition() {
	if (partition_) fclose(partition_);
	++slavePort_;
	++partIndex_;
  minnode_ = numNodes_ - 1;

	char partitionName[1024];
	sprintf(partitionName, "%s_%d.txt", outputFilePrefix_, partIndex_);

	partition_ = fopen(partitionName, "w");
  if (NULL == partition_) {
  	logger_->error("Error opening file %s.", partitionName);
  	return false;
  }

  return true;
}



