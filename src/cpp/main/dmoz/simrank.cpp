#include "simrank.h"

void SimRank::read(fstream& f) {
  char line[1024];
  long page, to_personalize;
  vector<long> path;
  for (int i = 0; i < num_pages_; ++i) {
    for (int j = 0; j < num_pathes_; ++j) {
      f.getline(line, 1024);
      stringstream ss (stringstream::in|stringstream::out);
      ss << line;
      ss >> to_personalize;
      if (j == 0) fromKeys_.push_back(to_personalize);
      //printf("%s\n", ss.str().c_str());
      path.clear();
      while (!ss.eof()) {
        ss >> page;
        path.push_back(page);
      }
      fingerprints_[to_personalize].push_back(path);
      //if (to_personalize == 76357 && j == 459) printf("%d\n", path[1]);
      //if (to_personalize == 77343 && j == 459) printf("%d\n", path[1]);
  //    printf("%ld\n", to_personalize);
    }
  }
/*  bool checkFirst =
    fingerprints_[76357][458][2] == 529301747 && fingerprints_[77343][458][2] == 529301747;
  printf("Check first: %d\n", checkFirst);*/
//  printf("%ld %ld\n", fingerprints_[76357][459][1], fingerprints_[77343][459][1]);
  printf("FPrints are stored.\n");
 /* for (table_it outer = fingerprints_.begin(); outer != fingerprints_.end(); ++outer) {
    printf("from %ld\n", outer->first);
    for (int i = 0; i < num_pathes_; ++i) {
      for (int j = 0; j < outer->second[i].size(); ++j) {
        printf("%ld ", outer->second[i][j]);
      }
      printf("\n");
    }
  }*/
}

void SimRank::needSR (fstream& f) {
  long egyik, masik;
  //double s;
  char line[1024];
  while (!f.eof()) {
    f.getline(line, 1024);
    stringstream ss (stringstream::in|stringstream::out);
    ss << line;
    ss >> egyik;
    ss >> masik;
    //f >> s;
    if (egyik == 76357 && masik == 77343) printf("Beolvassukuk: 76357 77343\n");
    need_score.push_back(std::make_pair<long, long>(egyik, masik));
  }
}

double SimRank::score (long self, long other) {
      //if (self == 76357 && other == 77343) printf("Megprobaljuk: 76357 77343\n");
      if (fingerprints_.find(self) == fingerprints_.end() || 
          fingerprints_.find(other) == fingerprints_.end()) return 0.0;

      //if (self == 76357 && other == 77343) printf("Van ilyen: 76357 77343\n");
      int len;
      vector<int> positions;
      positions.resize(path_len_);
      for (int path = 0; path < num_pathes_; ++path) {
        len = min (fingerprints_[self][path].size(), fingerprints_[other][path].size());
        int dist = -1;
        for (int pos = 0; pos < len; ++pos) {
          if (fingerprints_[self][path][pos] == fingerprints_[other][path][pos]) {
            //printf("found: %ld %ld %ld\n", self, path, pos);
            dist = pos;
            break;
          }
        }
        //if (self == 76357 && other == 77343 && path == 459) printf("Dist 459: %d\n", dist);
        if (dist >= 0) {
          ++positions[dist];
        }
      }
      double est = 0.0;
      for (int pos = 0; pos < path_len_; ++pos) {
        est += positions[pos] * pow(const_, pos+1) / num_pathes_;
      }
      return est;
}

void SimRank::compute() {
  fstream output_stream_(output_, fstream::out);
  double est;
  for (table_it outer = fingerprints_.begin(); outer != fingerprints_.end(); ++outer) {
    for (table_it inner = fingerprints_.begin(); inner != outer; ++inner)  {
      est = score(outer->first, inner->first);
//      result_[outer->first][inner->first] = est;
      output_stream_ << outer->first << " " << inner->first << " "
      << est << "\n";
    }
  }
  output_stream_.close();
}

void SimRank::computeNeedScore() {
  fstream output_stream_(output_, fstream::out);
  double est;
  //printf("output file: %s\n", output_);
  for (int i = 0; i < (int)need_score.size(); ++i) {
    if (i % 10000 == 0) printf("%d pair computed.\n", i);
    est = score(need_score[i].first, need_score[i].second);
    //printf("%ld %ld %lf\n", need_score[i].first, need_score[i].second, est);
    output_stream_ << need_score[i].first << " " << need_score[i].second << " "
      << est << "\n";
  }
  output_stream_.close();
}

inline long SimRank::nextRand() { 
  return fromKeys_[rand() % num_pages_];  
}

void SimRank::randomCompute() {
  fstream output_stream_(output_, fstream::out);

  srand ( time(NULL) );
  int size = (int) fingerprints_.size();
  int i = 0;
  double est;
  long randomSelf, randomOther;
  int iteration_num = 0;
  while (i < max_random_compute_) {
    ++iteration_num;
    if (iteration_num % 10000 == 0) printf("%d iteration done.\n",iteration_num);
    randomSelf = nextRand();
    randomOther = nextRand();
    if (randomSelf == randomOther) continue;
    est = score(randomSelf, randomOther);
    if (est > 0.0) {
      if (i % 10000 == 0) printf("%d pair computed.\n",i);
//      result_[randomSelf][randomOther] = est;
      output_stream_ << randomSelf << " " << randomOther << " " << est << "\n";
      ++i;
    }
  }
  output_stream_.close();
}

/*void SimRank::flush() {
  fstream output_stream(output_, fstream::out);
  printf("Flushing result.\n");
  for (unordered_map<long, unordered_map<long, double> >::iterator it = result_.begin(); 
          it != result_.end(); ++it) {
    for (unordered_map<long, double>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
      if (jt->second > 0.0) {
        output_stream << it->first << "\t" << jt->first << "\t" << jt->second << "\n";
      }
    }
  }
  output_stream.close();
}*/

void SimRank::setOutputStream(char* fname) {
  strcpy(output_, fname);
}
