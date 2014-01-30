import unittest
import sys
import math

sys.path.append('../../main/simrank/')
from count_tree_size import TreeCounter

class TestTreeSize(unittest.TestCase):
  def setUp(self):
    pass

  def test_process_line(self):
    line = "x 1 2 3 4 5\n"
    treeCounter = TreeCounter()
    treeCounter.processLine(line)
    self.assertEqual(len(treeCounter.nodes), 4)

    line = "x 1 2 4 3 5\n"
    treeCounter.processLine(line)
    print treeCounter.nodes 
    self.assertEqual(len(treeCounter.nodes), 7)

if __name__ == '__main__':
    unittest.main(argv=[sys.argv[0]])

