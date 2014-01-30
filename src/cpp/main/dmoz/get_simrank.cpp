#include <iostream>
#include "simrank.h"

void usage() {
  printf("Number of pages|length of path|number of pathes|dump?|fingerprints|needed pairs|output\n");
}


int main (int argc, char * argv[]) {
  if (argv[1] == NULL) {
    usage();
    exit(0);
  }
  SimRank sr(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atof(argv[4]));
  fstream in(argv[5], fstream::in);
  sr.read(in);
  fstream needSR(argv[6], fstream::in);

  sr.needSR(needSR);
  sr.setOutputStream(argv[7]);

  sr.computeNeedScore();
//  fstream out(argv[6], fstream::out);
  //sr.flush();
  in.close();
  //out.close();
}

      /*positions.clear();
      positions.resize(path_len_);
      for (int path = 0; path < num_pathes_; ++path) {
        len = min (outer->second[path].size(), inner->second[path].size());
        int dist = -1;
        for (int pos = 0; pos < len; ++pos) {
          if (outer->second[path][pos] == inner->second[path][pos]) {
            dist = pos;
            break;
          }
        }
        if (dist >= 0) {
          ++positions[dist];
        }
      }
      double est = 0.0;
      for (int pos = 0; pos < path_len_; ++pos) {
        est += positions[pos] * pow(const_, pos+1) / num_pathes_;
      }
      result_[outer->first][inner->first] = est;*/

