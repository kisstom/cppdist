/*
 * cfg_reader.cpp
 *
 *  Created on: 2013.08.13.
 *      Author: kisstom
 */

#include <string>
#include <iostream>
#include <algorithm>
#include <string>
#include <string.h>
#include <stdio.h>
#include "cfg_reader.h"
using std::find;

void CfgReader::trim (string& input) {
  input.erase(std::remove(input.begin(),input.end(),' '),input.end());
  //std::replace_if(s.begin(), s.end(), " ", "");
}

void CfgReader::read(char* file) {
	FILE* f = fopen(file, "r");
	if (f == NULL) {
		printf("ERROR opening file %s.\n", file);
		return;
	}

	params_ = new unordered_map<string, string>();
	char line[1024];
	char key[1024];
	char value[1024];
	int size, prevSize;
	while (fgets(line, 1024, f) != NULL) {
		if (line[strlen(line) - 1] == '\n') {
			line[strlen(line) - 1] = '\0';
		}

		char* pos = find(line, line + strlen(line), '=');
		if (pos == line + strlen(line)) {
      //printf("%s\n", line);
      continue;
		}

		size = (int) (pos - line);

		strncpy(key, line, size);
		key[size] = '\0';

		prevSize = size + 1;
		size = (int) (strlen(line) - prevSize);

		strncpy(value, line + prevSize, size);
		value[size] = '\0';

		string k = string(key);
		string v = string(value);
		trim(k);
    trim(v);


		(*params_)[k] = v;
		//if (k.find("MASTER_LOG") != std::string::npos) {
		printf("key-value %s %s\n", k.c_str(), v.c_str());
		  //printf("|%s|\n", k.c_str());
		//}
	}


	fclose(f);
}


unordered_map<string, string>* CfgReader::getParams() {
	return params_;
}


