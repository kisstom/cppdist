#include "ConfigParser.h"

using namespace std;

void ConfigParser::parse() {
  FILE* f = fopen(config_file_.c_str(), "r");
  char line[1024];
  int jobs;
  while (fgets(line, 1024, f) != NULL) {
    string str_line(line);
    size_t pos1 = str_line.find_first_of("=");
    if (str_line.substr(0, pos1).compare("LOCAL_DIR") == 0) {
      local_dir = str_line.substr(pos1 + 1);
      if (local_dir[local_dir.size() - 1] == '\n') {
        local_dir.erase(local_dir.begin() + local_dir.size() - 1);
      }
    }
    if (str_line.substr(0, pos1).compare("MACHINE") == 0) {
      size_t pos2 = str_line.find_first_of("=", pos1 + 1);
      machines.push_back(str_line.substr(pos1 + 1, pos2 - pos1 - 1));
      sscanf(str_line.substr(pos2 + 1).c_str(), "%d", &jobs);
      numjobs.push_back(jobs);
    }
  }
  fclose(f);
}

vector<string> ConfigParser::getOutputFiles() {
  vector<string> res;
  int ind = 0;
  for (unsigned i = 0; i < machines.size(); ++i) {
    for (int j = 0; j < numjobs[i]; ++ind, ++j) {
       stringstream ss (stringstream::in | stringstream::out);
       ss << "/mnt/";
       ss << machines[i] + "/";
       ss << local_dir;
       ss << "/logs/out_" << ind;
       res.push_back(ss.str());
     }
  }
  return res;
}
