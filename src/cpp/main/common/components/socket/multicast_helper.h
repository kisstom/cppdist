#ifndef MULTICAST_HELPER_H_
#define MULTICAST_HELPER_H_

class MulticastHelper {
public:
  MulticastHelper(int);
  int publishHashId(short*, bool*);

private:
  int nodeIndex;
};


#endif  // MULTICAST_HELPER_H_
