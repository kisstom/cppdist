#ifndef IP_INDEX_MAKER_H_
#define IP_INDEX_MAKER_H_

#include <gtest/gtest_prod.h>

class IPIndexMaker {
public:
  IPIndexMaker(int, int);
  void init();
  int createPublisherIPIndex(int publisherIndex);
  int createClientIPIndex(int publisherIndex);
private:
  void setLowerBitExcluder();
  void setUpperBitExcluder();
  int lowerBitExcluder;
  int upperBitExcluder;

  int nodeIndex;
  int clusterSize;

  // FRIEND TESTS
  FRIEND_TEST(TestIPIndexMaker, testInit);
};


#endif  // IP_INDEX_MAKER_H_
