#include <string>
#include "merge.h"

using namespace std;

int main (int c, char* argv[]) {
  vector<string> res;
  for (int i = 1; i < c - 1; ++i) {
    res.push_back(string(argv[i]));
  }
 
  Merge merge(res, string(argv[c-1]));
  merge.merge();
}
