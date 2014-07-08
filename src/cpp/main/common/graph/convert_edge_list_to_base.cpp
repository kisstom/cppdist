/*
 * convert_edge_list_to_base.cpp
 *
 *  Created on: 2014.06.19.
 *      Author: kisstom
 */

#include "convert_edge_list_to_base.h"

void ConvertEdgeListToBase::read(char* inpF, char* outpF) {
  FILE* inp = fopen(inpF, "r");
  int from, to;
  unordered_map<int, vector<int> > mx;

  vector<int> proto;
  while (fscanf(inp, "%d %d\n", &from, &to) != EOF) {
    if (mx.find(from) == mx.end()) {
      mx[from] = proto;
    }

    mx[from].push_back(to);
  }
  fclose(inp);

  FILE* outp = fopen(outpF, "w");
  for (unordered_map<int, vector<int> >::iterator it = mx.begin(); it != mx.end(); ++it) {
    fprintf(outp, "%d", it->first);

    for (vector<int>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
      fprintf(outp, " %d", *jt);
    }

    fprintf(outp, "\n");
  }

  fclose(outp);
}



