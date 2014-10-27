#include "../../../main/common/graph_converter/shuffle_continue_reindexer.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream>

TEST(ShuffleContinueReindexerTest, test) {
  ShuffleContinueReindexer indexer("", "", "", 685230, 20);
  long randomIndex = 685195;

  indexer.nodeCounter = 685219;
  indexer.reindexFile = stderr;
  long reindex = indexer.reindexer(randomIndex);
  long expected = 685229;
  ASSERT_EQ(expected, reindex);
}

TEST(ShuffleContinueReindexerTest, test2) {
  ShuffleContinueReindexer indexer("", "", "", 20, 3);
  long randomIndex = 685195;

  indexer.nodeCounter = 17;
  indexer.reindexFile = stderr;
  long reindex = indexer.reindexer(randomIndex);
  long expected = 19;
  ASSERT_EQ(expected, reindex);

  indexer.nodeCounter = 18;
  expected = 6;
  reindex = indexer.reindexer(randomIndex + 1);
  ASSERT_EQ(expected, reindex);

  indexer.nodeCounter = 19;
  expected = 13;
  reindex = indexer.reindexer(randomIndex + 2);
  ASSERT_EQ(expected, reindex);

  reindex = indexer.reindexer(randomIndex + 2);
  ASSERT_EQ(expected, reindex);
}

TEST(ShuffleContinueReindexerTest, test3) {
  ShuffleContinueReindexer indexer("", "", "", 21, 3);
  long randomIndex = 685195;

  indexer.nodeCounter = 17;
  indexer.reindexFile = stderr;
  long reindex = indexer.reindexer(randomIndex);
  long expected = 19;
  ASSERT_EQ(expected, reindex);

  indexer.nodeCounter = 18;
  expected = 6;
  reindex = indexer.reindexer(randomIndex + 1);
  ASSERT_EQ(expected, reindex);

  indexer.nodeCounter = 19;
  expected = 13;
  reindex = indexer.reindexer(randomIndex + 2);
  ASSERT_EQ(expected, reindex);

  indexer.nodeCounter = 20;
  expected = 20;
  reindex = indexer.reindexer(randomIndex + 3);
  ASSERT_EQ(expected, reindex);
}

TEST(ShuffleContinueReindexerTest, test4) {
  ShuffleContinueReindexer indexer("", "", "", 27, 5);
  long randomIndex = 685195;

  indexer.nodeCounter = 16;
  indexer.reindexFile = stderr;
  long reindex = indexer.reindexer(randomIndex);
  long expected = 9;
  ASSERT_EQ(expected, reindex);

  indexer.nodeCounter = 17;
  reindex = indexer.reindexer(randomIndex + 1);
  expected = 15;
  ASSERT_EQ(expected, reindex);


  indexer.nodeCounter = 18;
  expected = 20;
  reindex = indexer.reindexer(randomIndex + 2);
  ASSERT_EQ(expected, reindex);

  indexer.nodeCounter = 19;
  expected = 25;
  reindex = indexer.reindexer(randomIndex + 3);
  ASSERT_EQ(expected, reindex);

  indexer.nodeCounter = 25;
  reindex = indexer.reindexer(randomIndex + 1);
  expected = 15;
  ASSERT_EQ(expected, reindex);
}

TEST(ShuffleContinueReindexerTest, test5) {
  ShuffleContinueReindexer indexer("", "", "", 27, 5);
  indexer.nodeCounter = 18;
  indexer.reindexFile = stderr;
  indexer.reindexer(0);
  ASSERT_EQ(19, indexer.nodeCounter);

  indexer.reindexer(0);
  ASSERT_EQ(19, indexer.nodeCounter);
}

/*TEST(ShuffleContinueReindexerTest, test6) {
  ShuffleContinueReindexer indexer("", "", "", 27, 5);
  indexer.nodeCounter = 18;
  indexer.reindexer(0);
  ASSERT_EQ(19, indexer.nodeCounter);
}*/

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
