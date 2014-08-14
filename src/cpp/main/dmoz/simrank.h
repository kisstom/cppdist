#include <iostream>
#include <vector>
#include <fstream>
#include <tr1/unordered_map>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

using namespace std;
using std::vector;
using std::tr1::unordered_map;
using std::stringstream;
using std::fstream;
using std::min;
using std::pair;
using std::string;

typedef unordered_map<long, vector<vector<long> > >::iterator table_it;

class SimRank {
  public:
      SimRank(int npg, int pl, int npt, double c, int mrc = 0) : 
          num_pages_(npg), path_len_(pl), num_pathes_(npt), const_(c),
          max_random_compute_(mrc){} 
      void read(fstream& f);
      void computeNeedScore();
      void needSR(fstream& f);
      double score(long self, long other);
      void setOutputStream(char* fname);
      void setFringerPrint(unordered_map<long, vector<vector<long>
          > > fp) {fingerprints_ = fp;}
  private:
      long nextRand();
      int num_pages_;
      int path_len_;
      int num_pathes_;
      double const_;
      unordered_map<long, vector<vector<long> > > fingerprints_;
      unordered_map<long, unordered_map<long, double> > result_;
      vector<pair<long, long> > need_score;
      vector<long> fromKeys_;
      int max_random_compute_;
      char output_[1024];
};

