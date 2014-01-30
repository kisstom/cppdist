/*
 * remove_paralel.h
 *
 *  Created on: 2013.11.05.
 *      Author: kisstom
 */

#ifndef REMOVE_PARALEL_H_
#define REMOVE_PARALEL_H_

#include <iostream>
#include <vector>
#include <set>
#include <string.h>
#include <stdio.h>

using std::vector;
using std::set;
using std::istream;
using std::iostream;
using std::string;

class RemoveParalel {
public:
	RemoveParalel(int bufferSize);
	bool processNext( istream&);
	bool processLine( istream&);
	void process(istream& input, iostream& line);
	bool parseNextLine(istream& input, iostream& line);
	void clear(iostream& line);


	void processLine(char*);
	void process(string input, string output);
	vector<long>& getProcessedLine();
private:
	vector<long> listEdges_;
	set<long> setEdges_;
	int bufferSize_;
	char stringEdge_[16];
	long edge_;
};


#endif /* REMOVE_PARALEL_H_ */
