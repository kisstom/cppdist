#include <iostream>
#include "../../../../main/common/util/cfg_reader.h"

using std::cout;

int main (int argc, char* argv[]) {
  CfgReader reader;
  reader.read(argv[1]);
  unordered_map<string, string>* hostAndPort = reader.getHostAndPort();

  for (unordered_map<string, string>::iterator it = hostAndPort->begin();
      it != hostAndPort->end(); ++it) {
    cout << it->first << " " << it->second << "\n";
  }
}
