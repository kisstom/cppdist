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

      path.clear();
      while (!ss.eof()) {
        ss >> page;
        path.push_back(page);
      }
      fingerprints_[to_personalize].push_back(path);

    }
  }

  printf("FPrints are stored.\n");
}

void SimRank::needSR (fstream& f) {
  long egyik, masik;
  char line[1024];
  while (f.peek() != EOF) {
    f.getline(line, 1024);
    stringstream ss (stringstream::in|stringstream::out);
    ss << line;
    ss >> egyik;
    ss >> masik;
    need_score.push_back(std::make_pair<long, long>(egyik, masik));
  }
}

double SimRank::score (long self, long other) {
      if (fingerprints_.find(self) == fingerprints_.end() || 
          fingerprints_.find(other) == fingerprints_.end()) return 0.0;

      int len;
      vector<int> positions;
      positions.resize(path_len_);
      for (int path = 0; path < num_pathes_; ++path) {
        len = min (fingerprints_[self][path].size(), fingerprints_[other][path].size());
        int dist = -1;
        for (int pos = 0; pos < len; ++pos) {
          if (fingerprints_[self][path][pos] == fingerprints_[other][path][pos]) {
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
        est += positions[pos] * pow(const_, pos) / num_pathes_;
      }
      return est;
}

void SimRank::computeNeedScore() {
  fstream output_stream_(output_, fstream::out);
  double est;

  for (int i = 0; i < (int)need_score.size(); ++i) {
    if (i % 10000 == 0) printf("%d pair computed.\n", i);
    est = score(need_score[i].first, need_score[i].second);
    output_stream_ << need_score[i].first << " " << need_score[i].second << " "
      << est << "\n";
  }
  output_stream_.close();
}

void SimRank::setOutputStream(char* fname) {
  strcpy(output_, fname);
}
