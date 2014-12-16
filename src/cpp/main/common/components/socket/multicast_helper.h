#ifndef MULTICAST_HELPER_H_
#define MULTICAST_HELPER_H_

#include <set>

using std::set;

class MulticastHelper {
public:
  MulticastHelper(int);
  int publishHashId(short*, bool*);
  int publishHashId(const set<short>&, bool*);

private:
  int nodeIndex;
};


#endif  // MULTICAST_HELPER_H_
