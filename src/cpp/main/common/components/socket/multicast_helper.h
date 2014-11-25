#ifndef MULTICAST_HELPER_H_
#define MULTICAST_HELPER_H_

class MulticastHelper {
public:
  MulticastHelper(int);
  int hash(short*);

private:
  int nodeIndex;
};


#endif  // MULTICAST_HELPER_H_
