import unittest
import sys
import math

class TestRMSEDistance(unittest.TestCase):
  def setUp(self):
    self.dist_pr = []
    f = open(sys.argv[1])
    for line in f:
      self.dist_pr += [float(line.strip().split()[1])]
    f.close()

    self.seq_pr = []
    f = open(sys.argv[2])
    for line in f:
      self.seq_pr += [float(line.strip().split()[1])]
    f.close()
    self.deg = int(sys.argv[3])

  def test_rmse(self):
    self.assertTrue(len(self.dist_pr) == len(self.seq_pr))
    rmse = 0.0
    for i in xrange(len(self.dist_pr)):
      rmse += (self.dist_pr[i] - self.seq_pr[i])**2

    self.assertAlmostEqual(math.sqrt(rmse / len(self.dist_pr)), 0, self.deg)

if __name__ == '__main__':
    unittest.main(argv=[sys.argv[0]])

