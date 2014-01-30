/*
 * remove_paralel.cpp
 *
 *  Created on: 2013.11.05.
 *      Author: kisstom
 */

#include "remove_paralel.h"

RemoveParalel::RemoveParalel(int bufferSize) {
	bufferSize_ = bufferSize;
}

bool RemoveParalel::processNext(istream& line) {
  if (line.eof()) return false;
  int s = 16;
	line.getline(stringEdge_, s, ' ');
	//printf("edge %s\n", stringEdge_);
	sscanf(stringEdge_, "%ld", &edge_);
	if (setEdges_.find(edge_) == setEdges_.end()) {
    listEdges_.push_back(edge_);
    setEdges_.insert(edge_);
  }
	return true;
}

bool RemoveParalel::processLine(istream& line) {
	bool hasMore = true;
	while (hasMore) {
		hasMore = processNext(line);
	}
	return false;
}

bool RemoveParalel::parseNextLine(istream& input, iostream& line) {
	if (input.eof()) return false;

	char buf[bufferSize_];
	while (1) {
		input.getline(buf, bufferSize_, '\n');
		printf("buf %s\n", buf);
		line << buf;
		if (!input.fail()) break;
		else break;
	}
	return true;
}

void RemoveParalel::process(istream& input, iostream& line) {
	bool hasMore = true;
	while (hasMore) {
		hasMore = parseNextLine(input, line);
		if (!hasMore) return;
		processLine(line);
		//flushEdges();
    clear(line);
	}
}

void RemoveParalel::clear(iostream& line) {
	listEdges_.clear();
  setEdges_.clear();
  line.clear();
}

void RemoveParalel::processLine(char* line) {
	size_t last_space = -1;
	long next = -1;
	int retSize = 0;
  for (size_t pos = 0; pos < strlen(line); ++pos) {
    if (line[pos] == ' ' || line[pos] == '\n') {
    	retSize = sscanf(line + last_space + 1, "%ld", &next);
    	if (retSize < 0) break;
    	last_space = pos;
    	if (setEdges_.find(next) == setEdges_.end()) {
    		listEdges_.push_back(next);
    		setEdges_.insert(next);
    	}
    }
  }

}

void RemoveParalel::process(string input, string output) {
	char* buffer = new char[bufferSize_];
	FILE* inputFile = fopen(input.c_str(), "r");
	if (inputFile == NULL) {
		printf("ERROR opening file %s.\n", input.c_str());
	}
	FILE* outputFile = fopen(output.c_str(), "w");
	if (outputFile == NULL) {
		printf("ERROR opening file %s.\n", output.c_str());
	}

	int c = 0;
	while (fgets(buffer, bufferSize_, inputFile)) {
		++c;
		if (c % 1000000 == 0) printf("%d rows processed.\n", c);

		if (buffer[strlen(buffer) - 1] != '\n') {
			printf ("Maybe not the whole line read at line %d.\n", c);
		}

		processLine(buffer);
		if (listEdges_.size() > 0) {
			fprintf(outputFile, "%ld", listEdges_[0]);
		}

		for (int i = 1; i < (int) listEdges_.size(); ++i) {
			fprintf(outputFile, " %ld", listEdges_[i]);
		}

		fprintf(outputFile, "\n");
		listEdges_.clear();
		setEdges_.clear();
	}

	fclose(inputFile);
	fclose(outputFile);
}

vector<long>& RemoveParalel::getProcessedLine() {
	return listEdges_;
}
