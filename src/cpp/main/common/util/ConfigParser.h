#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;

class ConfigParser {
  public:
    ConfigParser(string  config_file) : config_file_(config_file) {}
    void parse();
    vector<string> getOutputFiles();
  private:
    vector<string> machines;
    vector<int> numjobs;
    string local_dir;
    string config_file_;
};
