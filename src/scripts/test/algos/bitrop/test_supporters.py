import sys, os

base = os.path.dirname(os.path.abspath(__file__)) 
sys.path.append(base + '/../../../main/algos/bitprop/')

from  supporters import Supporters
import unittest

class TestSupporters(unittest.TestCase):
	def test_supp(self):
		graph = [[], [0, 1], [1, 2], [2, 3]]
		supporters = Supporters()
		self.assertEqual(2, supporters.run(graph, 1, 0))
		self.assertEqual(3, supporters.run(graph, 2, 0))
		self.assertEqual(4, supporters.run(graph, 3, 0))

if __name__ == '__main__':
   unittest.main()
