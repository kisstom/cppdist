#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../../../main/common/graph/adjacency_list_iterator.h"

TEST(AdjacencyListIteratorTest, testAdd) {
  AdjacencyList<long> container;
  container.setMinnode(5);
  container.initContainers();

  container.addEdge(7, 3);
  container.addEdge(7, 5);
  container.addEdge(9, 1);
  container.addEdge(10, 2);
  container.setFinish();

  AdjacencyListIterator<long> iterator = container.createIterator();
  iterator.resetRow(7);
  ASSERT_TRUE(iterator.hasNext());
  ASSERT_EQ(3, iterator.next());

  ASSERT_TRUE(iterator.hasNext());
  ASSERT_EQ(5, iterator.next());
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
