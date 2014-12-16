#include <gtest/gtest.h>
#include "../../../main/common/components/socket/multicast_helper.h"
#include <cmath>


TEST(MulticastHelperTest, test1) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);

  short* indices = new short[3];
  bool shouldUpdateSelf = false;
  // size of the neighbors
  indices[0] = 2;
  indices[1] = 0;
  indices[2] = 3;

  ASSERT_EQ(4, helper.publishHashId(indices, &shouldUpdateSelf));
  ASSERT_FALSE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, test2) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  short* indices = new short[3];
  // size of the neighbors
  indices[0] = 2;
  indices[1] = 1;
  indices[2] = 3;

  ASSERT_EQ(3, helper.publishHashId(indices, &shouldUpdateSelf));
  ASSERT_TRUE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, test3) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  short* indices = new short[3];
  // size of the neighbors
  indices[0] = 1;
  indices[1] = 1;

  ASSERT_EQ(-1, helper.publishHashId(indices, &shouldUpdateSelf));
  ASSERT_TRUE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, test4) {
  int nodeIndex = 2;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  short* indices = new short[4];
  // size of the neighbors
  indices[0] = 3;
  indices[1] = 1;
  indices[2] = 3;
  indices[3] = 4;

  ASSERT_EQ(13, helper.publishHashId(indices, &shouldUpdateSelf));
  ASSERT_FALSE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, testEmpty) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  short* indices = new short[1];
  // size of the neighbors
  indices[0] = 0;

  ASSERT_EQ(-1, helper.publishHashId(indices, &shouldUpdateSelf));
  ASSERT_FALSE(shouldUpdateSelf);
}

/***** TEST FOR SET *****/


TEST(MulticastHelperTest, testSet1) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);

  bool shouldUpdateSelf = false;
  set<short> myset;
  myset.insert(0);
  myset.insert(3);

  ASSERT_EQ(4, helper.publishHashId(myset, &shouldUpdateSelf));
  ASSERT_FALSE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, testSet2) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;

  set<short> myset;
  myset.insert(1);
  myset.insert(3);

  ASSERT_EQ(3, helper.publishHashId(myset, &shouldUpdateSelf));
  ASSERT_TRUE(shouldUpdateSelf);
}


TEST(MulticastHelperTest, testSet3) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  set<short> myset;
  myset.insert(1);

  ASSERT_EQ(-1, helper.publishHashId(myset, &shouldUpdateSelf));
  ASSERT_TRUE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, testSet4) {
  int nodeIndex = 2;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  set<short> myset;
  myset.insert(1);
  myset.insert(3);
  myset.insert(4);

  ASSERT_EQ(13, helper.publishHashId(myset, &shouldUpdateSelf));
  ASSERT_FALSE(shouldUpdateSelf);
}

TEST(MulticastHelperTest, testSet5) {
  int nodeIndex = 1;
  MulticastHelper helper(nodeIndex);
  bool shouldUpdateSelf = false;
  set<short> myset;

  ASSERT_EQ(-1, helper.publishHashId(myset, &shouldUpdateSelf));
  ASSERT_FALSE(shouldUpdateSelf);
}


int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
