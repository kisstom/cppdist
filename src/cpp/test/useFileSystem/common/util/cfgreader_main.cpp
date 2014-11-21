#include <iostream>
#include "../../../../main/common/util/cfg_reader.h"

using std::cout;

int main (int argc, char* argv[]) {
  CfgReader reader;
  reader.read(argv[1]);
  vector<std::pair<string, string> >* hostAndPort = reader.getHostAndPort();

  for (vector<std::pair<string, string> >::iterator it = hostAndPort->begin();
      it != hostAndPort->end(); ++it) {
    cout << it->first << " " << it->second << "\n";
  }
}
