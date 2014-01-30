#include <iostream>
#include "../util/ConfigParser.h"
#include "serialize.h"

int main (int argc, char* argv[]) {
  ConfigParser cp(argv[1]);
  cp.parse();
  vector<string> outs = cp.getOutputFiles();
  SerializeResult sr;
  sr.SerializeAll(outs, string(argv[2]));
  //sr.Deserialize(string(argv[2]), string(argv[3]));
}
