#include <gtest/gtest.h>
#include "../../main/dmoz/dmoz_reader.h"

TEST(CalculatorTest, first) {
  string t1("a/b/c1");
  string t2("a/b/c2");
  EXPECT_EQ(DmozReader::ComputeDistance(t1, t2), 2);

  t1 = string("a/x/c1");
  t2 = string("a/b/c2");
  EXPECT_EQ(DmozReader::ComputeDistance(t1, t2), 4);

  t1 = string("a/x/c1");
  t2 = string("a/c2");
  EXPECT_EQ(DmozReader::ComputeDistance(t1, t2), 3);

  t1 = string("a/x/c1");
  t2 = string("a/b/c1");
  EXPECT_EQ(DmozReader::ComputeDistance(t1, t2), 4);

  t1 = string("a/b/c1");
  t2 = string("a/b/c1");
  EXPECT_EQ(DmozReader::ComputeDistance(t1, t2), 0);

  t1 = string("a");
  t2 = string("a");
  EXPECT_EQ(DmozReader::ComputeDistance(t1, t2), 0);

  t1 = string("a/b");
  t2 = string("a");
  EXPECT_EQ(DmozReader::ComputeDistance(t1, t2), 1);

}

TEST(RootDistanceCalculatorTest, first) {
  string t1("a/b/c1");
  string t2("a/b/c2");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 2);

  t1 = string("a/x/c1");
  t2 = string("a/b/c2");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 1);

  t1 = string("a/x/c1");
  t2 = string("a/c2");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 1);

  t1 = string("a/x/c1");
  t2 = string("a/b/c1");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 1);

  t1 = string("a/b/c1");
  t2 = string("a/b/c1");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 3);

  t1 = string("a");
  t2 = string("a");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 1);

  t1 = string("a/b");
  t2 = string("a");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 1);

  t1 = string("a/b/c/x");
  t2 = string("a/b/x");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 2);

  t1 = string("y/b/c/x");
  t2 = string("a/b/x");
  EXPECT_EQ(DmozReader::ComputeDistanceFromRoot(t1, t2), 0);

}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

